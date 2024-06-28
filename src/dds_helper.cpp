#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <map>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/rtps/transport/shared_mem/SharedMemTransportDescriptor.h>
#include "dds_helper.h"
#include "log.h"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps::rtps;

DDSHelper::DDSHelper()
{
    this->init();
}


DDSHelper::DDSHelper(std::string name):m_name(name)
{
    this->init();
}


DDSHelper::~DDSHelper() 
{
    for (auto w: writer_map_) {
        if (w.second != nullptr) {
            publisher_->delete_datawriter(w.second);
        }
    }

    for(auto r: reader_list_) {
        subscriber_->delete_datareader(r);
    }

    for (auto t: topic_list_) {
        if (t != nullptr) {
            participant_->delete_topic(t);
        }
    }

    // TypeSupport 继承于智能指针，不需要主动delete

    for(auto s: sub_listensers_) {
        delete s;
    }

    if (subscriber_ != nullptr) {
        participant_->delete_subscriber(subscriber_);
    }

    if (publisher_ != nullptr) {
        participant_->delete_publisher(publisher_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}


bool DDSHelper::init() 
{

    DomainParticipantQos pqos;
    pqos.name(m_name);

    // // Explicit configuration of SharedMem transport
    // pqos.transport().use_builtin_transports = false;
    // auto shm_transport = std::make_shared<SharedMemTransportDescriptor>();
    // shm_transport->segment_size(2 * 1024 * 1024);
    // pqos.transport().user_transports.push_back(shm_transport);

    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

    if (nullptr == participant_) {
        return false;
    }

    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (nullptr == publisher_) {
        return false;
    }

    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
    if (subscriber_ == nullptr){
        return false;
    }

    return true;
}


bool DDSHelper::registe_writer(const std::string& topic_name,  void* data_type)
{
    if (data_type == nullptr) {
        return false;
    }

    TypeSupport new_type((TopicDataType*)data_type);
    new_type.register_type(participant_);
    type_list_.push_back(new_type);

    LOG_DEBUG("Create Publisher ok. topic_name = %s, type_name = %s\n", topic_name.c_str(), new_type.get_type_name().c_str());
    eprosima::fastdds::dds::Topic* new_topic = participant_->create_topic(topic_name, new_type.get_type_name(), TOPIC_QOS_DEFAULT);
    if (nullptr == new_topic) {
        return false;
    }
    topic_list_.push_back(new_topic);


    DataWriterQos wqos;
    wqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    wqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
    wqos.history().kind = KEEP_LAST_HISTORY_QOS;
    wqos.history().depth = 20;

    // wqos.resource_limits().max_samples = 10;
    // wqos.resource_limits().max_instances = 1;
    // wqos.resource_limits().max_samples_per_instance = 10;
    // wqos.resource_limits().allocated_samples = 0;
    // wqos.endpoint().history_memory_policy = eprosima::fastrtps::rtps::DYNAMIC_REUSABLE_MEMORY_MODE;

    auto new_writer = publisher_->create_datawriter(new_topic, wqos, &listener_);
    if (nullptr == new_writer) {
        return false;
    }
    writer_map_[topic_name]=new_writer;
    
    return true;
}

bool DDSHelper::registe_reader(const std::string &topic_name,  void* data_type, readerCallback func)
{
    if (data_type == nullptr) {
        return false;
    }

    TypeSupport new_type((TopicDataType*)data_type);
    new_type.register_type(participant_);
    type_list_.push_back(new_type);

    eprosima::fastdds::dds::Topic* new_topic = participant_->create_topic(topic_name, new_type.get_type_name(), TOPIC_QOS_DEFAULT);
    if (nullptr == new_topic) {
        return false;
    }
    topic_list_.push_back(new_topic);

    auto reader_listener = new DDSHelper::SubListener(func);

    DataReaderQos rqos;
    rqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    rqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
    rqos.history().kind = KEEP_LAST_HISTORY_QOS;
    rqos.history().depth = 20;

    // rqos.resource_limits().max_samples = 10;
    // rqos.resource_limits().max_instances = 1;
    // rqos.resource_limits().max_samples_per_instance = 10;
    // rqos.resource_limits().allocated_samples = 0;
    // rqos.endpoint().history_memory_policy = eprosima::fastrtps::rtps::DYNAMIC_REUSABLE_MEMORY_MODE;

    auto new_reader = subscriber_->create_datareader(new_topic, rqos, reader_listener);
    if (nullptr == new_reader) {
        return false;
    }
    reader_list_.push_back(new_reader);
    sub_listensers_.push_back(reader_listener);

    LOG_DEBUG("Create Subscriber ok. topic_name = %s, type_name = %s\n", topic_name.c_str(), new_type.get_type_name().c_str());


    return true;
}



void DDSHelper::PubListener::on_publication_matched(
    DataWriter* writer, const PublicationMatchedStatus& info) 
{
    if (1 == info.current_count_change) {
        LOG_DEBUG("publication matched: %s, total_count = %d\n", writer->get_type().get_type_name().c_str(),
               info.total_count);
    } else if (-1 == info.current_count_change) {
        LOG_DEBUG("publication unmatched: %s, total_count = %d\n", writer->get_type().get_type_name().c_str(),
               info.total_count);
    } else {
        std::string log_info = std::to_string(info.current_count_change) +
                               "invalid value for PublicationMatchedStatus current count change";
        LOG_WARN("%s\n", log_info.c_str());
    }
}


void DDSHelper::PubListener::on_offered_deadline_missed(
        DataWriter* writer,
        const OfferedDeadlineMissedStatus& status)
{
    (void)writer;
    LOG_WARN("Deadline missed: total_count=%d, total_count_change=%d\n", 
        status.total_count, status.total_count_change);
    
}

void DDSHelper::PubListener::on_offered_incompatible_qos(
    eprosima::fastdds::dds::DataWriter* writer,
    const eprosima::fastdds::dds::OfferedIncompatibleQosStatus& status) 
{
    (void)writer;
    LOG_WARN("on_offered_incompatible_qos: last_policy_id=%d\n", status.last_policy_id);
}

void DDSHelper::PubListener::on_liveliness_lost(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::LivelinessLostStatus& status) 
{
    (void)writer;
    LOG_DEBUG("on_liveliness_lost: total count=%d\n", status.total_count);
}


DDSHelper::SubListener::SubListener(readerCallback func): func_(func)
{
}


void DDSHelper::SubListener::on_subscription_matched (
    DataReader* reader, const SubscriptionMatchedStatus& info) 
{
    if (info.current_count_change == 1)
    {
        LOG_DEBUG("Subscriber matched: %s, total_count = %d\n", 
            reader->type().get_type_name().c_str(),info.total_count);
    }
    else if (info.current_count_change == -1)
    {
        LOG_DEBUG("Subscriber unmatched: %s, total_count = %d\n", 
            reader->type().get_type_name().c_str(), info.total_count);
    }
    else
    {
        LOG_WARN("invalid value for SubscriptionMatchedStatus : %d\n", 
            info.current_count_change);
    }
}


void DDSHelper::SubListener::on_requested_deadline_missed(
        DataReader* reader,
        const RequestedDeadlineMissedStatus& status)
{
    (void)reader;
    LOG_WARN("Deadline missed: total_count=%d, total_count_change=%d\n", 
        status.total_count, status.total_count_change);
}


void DDSHelper::SubListener::on_requested_incompatible_qos(
    DataReader* reader,
    const RequestedIncompatibleQosStatus& status) 
    {
    (void)reader;
    LOG_WARN("on_requested_incompatible_qos: last_policy_id=%d\n", status.last_policy_id);
}

void DDSHelper::SubListener::on_sample_lost(
    eprosima::fastdds::dds::DataReader* reader, 
    const eprosima::fastdds::dds::SampleLostStatus& status) 
{
    LOG_WARN("on_sample_lost\n");
}



void DDSHelper::SubListener::on_sample_rejected(
    eprosima::fastdds::dds::DataReader* reader, 
    const eprosima::fastdds::dds::SampleRejectedStatus& status)
{
    LOG_WARN("on_sample_rejected\n");
}

void DDSHelper::SubListener::on_liveliness_changed(
    eprosima::fastdds::dds::DataReader* reader, 
const eprosima::fastdds::dds::LivelinessChangedStatus& status) 
{
    LOG_WARN("on_liveliness_changed");
}



void DDSHelper::SubListener::on_data_available(DataReader *reader)
{
    auto data =reader->type().create_data();
    SampleInfo info;

    if (reader->take_next_sample(data, &info) == ReturnCode_t::RETCODE_OK){
        if (info.valid_data){
            func_(data);
            reader->type().delete_data(data);
        }
    }

   return;
}



void DDSHelper::publish(std::string topic, void* msg) 
{
    if (writer_map_.find(topic) != writer_map_.end()){
        writer_map_[topic]->write(msg);
    } else {
        LOG_WARN("can not find topic: %s\n", topic.c_str());
    }
}