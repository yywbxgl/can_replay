// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file CanFrame.cpp
 * This source file contains the implementation of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "CanFrame.h"

#include <fastcdr/Cdr.h>


#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>




CanFrame::CanFrame()
{
}

CanFrame::~CanFrame()
{
}

CanFrame::CanFrame(
        const CanFrame& x)
{
    m_timestamp = x.m_timestamp;
    m_id = x.m_id;
    m_dlc = x.m_dlc;
    m_reserved = x.m_reserved;
    m_data = x.m_data;
}

CanFrame::CanFrame(
        CanFrame&& x) noexcept
{
    m_timestamp = x.m_timestamp;
    m_id = x.m_id;
    m_dlc = x.m_dlc;
    m_reserved = std::move(x.m_reserved);
    m_data = std::move(x.m_data);
}

CanFrame& CanFrame::operator =(
        const CanFrame& x)
{

    m_timestamp = x.m_timestamp;
    m_id = x.m_id;
    m_dlc = x.m_dlc;
    m_reserved = x.m_reserved;
    m_data = x.m_data;
    return *this;
}

CanFrame& CanFrame::operator =(
        CanFrame&& x) noexcept
{

    m_timestamp = x.m_timestamp;
    m_id = x.m_id;
    m_dlc = x.m_dlc;
    m_reserved = std::move(x.m_reserved);
    m_data = std::move(x.m_data);
    return *this;
}

bool CanFrame::operator ==(
        const CanFrame& x) const
{
    return (m_timestamp == x.m_timestamp &&
           m_id == x.m_id &&
           m_dlc == x.m_dlc &&
           m_reserved == x.m_reserved &&
           m_data == x.m_data);
}

bool CanFrame::operator !=(
        const CanFrame& x) const
{
    return !(*this == x);
}

/*!
 * @brief This function sets a value in member timestamp
 * @param _timestamp New value for member timestamp
 */
void CanFrame::timestamp(
        uint64_t _timestamp)
{
    m_timestamp = _timestamp;
}

/*!
 * @brief This function returns the value of member timestamp
 * @return Value of member timestamp
 */
uint64_t CanFrame::timestamp() const
{
    return m_timestamp;
}

/*!
 * @brief This function returns a reference to member timestamp
 * @return Reference to member timestamp
 */
uint64_t& CanFrame::timestamp()
{
    return m_timestamp;
}


/*!
 * @brief This function sets a value in member id
 * @param _id New value for member id
 */
void CanFrame::id(
        uint32_t _id)
{
    m_id = _id;
}

/*!
 * @brief This function returns the value of member id
 * @return Value of member id
 */
uint32_t CanFrame::id() const
{
    return m_id;
}

/*!
 * @brief This function returns a reference to member id
 * @return Reference to member id
 */
uint32_t& CanFrame::id()
{
    return m_id;
}


/*!
 * @brief This function sets a value in member dlc
 * @param _dlc New value for member dlc
 */
void CanFrame::dlc(
        uint8_t _dlc)
{
    m_dlc = _dlc;
}

/*!
 * @brief This function returns the value of member dlc
 * @return Value of member dlc
 */
uint8_t CanFrame::dlc() const
{
    return m_dlc;
}

/*!
 * @brief This function returns a reference to member dlc
 * @return Reference to member dlc
 */
uint8_t& CanFrame::dlc()
{
    return m_dlc;
}


/*!
 * @brief This function copies the value in member reserved
 * @param _reserved New value to be copied in member reserved
 */
void CanFrame::reserved(
        const std::array<uint8_t, 3>& _reserved)
{
    m_reserved = _reserved;
}

/*!
 * @brief This function moves the value in member reserved
 * @param _reserved New value to be moved in member reserved
 */
void CanFrame::reserved(
        std::array<uint8_t, 3>&& _reserved)
{
    m_reserved = std::move(_reserved);
}

/*!
 * @brief This function returns a constant reference to member reserved
 * @return Constant reference to member reserved
 */
const std::array<uint8_t, 3>& CanFrame::reserved() const
{
    return m_reserved;
}

/*!
 * @brief This function returns a reference to member reserved
 * @return Reference to member reserved
 */
std::array<uint8_t, 3>& CanFrame::reserved()
{
    return m_reserved;
}


/*!
 * @brief This function copies the value in member data
 * @param _data New value to be copied in member data
 */
void CanFrame::data(
        const std::array<uint8_t, 8>& _data)
{
    m_data = _data;
}

/*!
 * @brief This function moves the value in member data
 * @param _data New value to be moved in member data
 */
void CanFrame::data(
        std::array<uint8_t, 8>&& _data)
{
    m_data = std::move(_data);
}

/*!
 * @brief This function returns a constant reference to member data
 * @return Constant reference to member data
 */
const std::array<uint8_t, 8>& CanFrame::data() const
{
    return m_data;
}

/*!
 * @brief This function returns a reference to member data
 * @return Reference to member data
 */
std::array<uint8_t, 8>& CanFrame::data()
{
    return m_data;
}


// Include auxiliary functions like for serializing/deserializing.
#include "CanFrameCdrAux.ipp"

