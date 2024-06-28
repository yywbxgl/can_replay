#pragma once


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
#include <fastdds/dds/topic/TypeSupport.hpp>

#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>


using namespace eprosima::fastdds::dds;
using readerCallback = std::function<void(void*)>;

class DDSHelper
{
public:
    DDSHelper();
    DDSHelper(std::string name);

    virtual ~DDSHelper();

    bool registe_writer(const std::string& topic_name,  void* data_type) ;

    bool registe_reader(const std::string& topic_name,  void* data_type, readerCallback func) ;

    void publish(std::string topic, void* msg) ;

private:
    bool init();

private:
    class SubListener : public eprosima::fastdds::dds::DataReaderListener{
    public:
        SubListener(readerCallback func);
        ~SubListener() override = default;
        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;

        void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;
         void on_requested_deadline_missed(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::RequestedDeadlineMissedStatus& status) override;

         void on_requested_incompatible_qos(
            eprosima::fastdds::dds::DataReader* reader,
            const eprosima::fastdds::dds::RequestedIncompatibleQosStatus& status) override;

        void on_sample_lost(
            eprosima::fastdds::dds::DataReader* reader, 
            const eprosima::fastdds::dds::SampleLostStatus& status) override;

        void on_sample_rejected(
            eprosima::fastdds::dds::DataReader* reader, 
            const eprosima::fastdds::dds::SampleRejectedStatus& status) override;

        void on_liveliness_changed(
            eprosima::fastdds::dds::DataReader* reader, 
            const eprosima::fastdds::dds::LivelinessChangedStatus& status) override;

        readerCallback func_;
    };

private:
    class PubListener : public DataWriterListener {
    public:
        PubListener() = default;
        ~PubListener() = default;
        void on_publication_matched(DataWriter* writer,
            const PublicationMatchedStatus& info) override;

        void on_offered_deadline_missed(
            eprosima::fastdds::dds::DataWriter* writer,
            const eprosima::fastdds::dds::OfferedDeadlineMissedStatus& status) override;

        void on_offered_incompatible_qos(
            eprosima::fastdds::dds::DataWriter* writer,
            const eprosima::fastdds::dds::OfferedIncompatibleQosStatus& status) override;

        void on_liveliness_lost(
            eprosima::fastdds::dds::DataWriter* writer,
            const eprosima::fastdds::dds::LivelinessLostStatus& status) override;

    } listener_;

private:
    std::string m_name="dds_helper";
    DomainParticipant* participant_ = nullptr;
    Publisher* publisher_ = nullptr;
    Subscriber* subscriber_ = nullptr;
    std::map<std::string, DataWriter*> writer_map_;
    std::vector<Topic*> topic_list_;
    std::vector<TypeSupport> type_list_;
    std::vector<SubListener*> sub_listensers_;
    std::vector<DataReader*> reader_list_;
};
