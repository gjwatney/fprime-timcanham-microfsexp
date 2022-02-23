/**
 * \file
 * \author T. Canham
 * \brief Implementation for telemetry channel component thread.
 *
 * \copyright
 * Copyright 2009-2022, by the California Institute of Technology.
 * ALL RIGHTS RESERVED.  United States Government Sponsorship
 * acknowledged.
 * <br /><br />
 */

#include <Svc/TlmChan/TlmChanImpl.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/Assert.hpp>
#include <Fw/Com/ComBuffer.hpp>
#include <Fw/Com/ComPacket.hpp>

namespace Svc {

    void TlmChanImpl::Run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
        // Only write packets if connected
        if (not this->isConnected_PktSend_OutputPort(0)) {
            return;
        }

        // lock mutex long enough to modify active telemetry buffer
        // so the data can be read without worrying about updates
        this->lock();
        this->m_activeBuffer = 1 - this->m_activeBuffer;
        // set activeBuffer to not updated
        for (U32 entry = 0; entry < TLMCHAN_HASH_BUCKETS; entry++) {
            this->m_tlmEntries[this->m_activeBuffer].buckets[entry].updated = false;
        }
        this->unLock();

        // initialize ComBuffer with packet type
        this->m_comBuffer.resetSer();
        Fw::SerializeStatus stat = this->m_comBuffer.serialize(static_cast<FwPacketDescriptorType>(Fw::ComPacket::FW_PACKET_TELEM));
        FW_ASSERT(Fw::FW_SERIALIZE_OK == stat,static_cast<NATIVE_INT_TYPE>(stat));

        // go through each entry and send a packet if it has been updated

        for (U32 entry = 0; entry < TLMCHAN_HASH_BUCKETS; entry++) {
            TlmEntry* p_entry = &this->m_tlmEntries[1-this->m_activeBuffer].buckets[entry];
            if ((p_entry->updated) && (p_entry->used)) {
                // check to see if there is room left in ComBuffer
                if ((sizeof(p_entry->id) + p_entry->lastUpdate.SERIALIZED_SIZE + p_entry->buffer.SERIALIZED_SIZE) >
                    (this->m_comBuffer.getBuffLeft())) {
                    // If not enough room left, send the current ComBuffer
                    this->PktSend_out(0,this->m_comBuffer,0);
                    // reinitialize comBuffer
                    this->m_comBuffer.resetSer();
                    this->m_comBuffer.serialize(static_cast<FwPacketDescriptorType>(Fw::ComPacket::FW_PACKET_TELEM));
                }
                // serialize channel data
                stat = this->m_comBuffer.serialize(p_entry->id);
                FW_ASSERT(Fw::FW_SERIALIZE_OK == stat,static_cast<NATIVE_INT_TYPE>(stat));
                stat = this->m_comBuffer.serialize(p_entry->lastUpdate);
                FW_ASSERT(Fw::FW_SERIALIZE_OK == stat,static_cast<NATIVE_INT_TYPE>(stat));
                stat = this->m_comBuffer.serialize(p_entry->buffer);
                FW_ASSERT(Fw::FW_SERIALIZE_OK == stat,static_cast<NATIVE_INT_TYPE>(stat));
                p_entry->updated = false;
            }
        }

        // if the ComBuffer is partially filled, send it
        if (this->m_comBuffer.getBuffLength() > 0) {
            this->PktSend_out(0,this->m_comBuffer,0);
        }
    }
}
