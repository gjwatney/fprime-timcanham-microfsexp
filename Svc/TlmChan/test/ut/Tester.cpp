// ======================================================================
// \title  TlmChan.hpp
// \author tcanham
// \brief  cpp file for TlmChan test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "Tester.hpp"
#include <Fw/Test/UnitTest.hpp>
#include <Fw/Com/ComPacket.hpp>

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10
#define QUEUE_DEPTH 10

namespace Svc
{

    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    Tester ::
        Tester(void) :
#if FW_OBJECT_NAMES == 1
                       TlmChanGTestBase("Tester", MAX_HISTORY_SIZE),
                       component("TlmChan")
#else
                       TlmChanGTestBase(MAX_HISTORY_SIZE),
                       component()
#endif
    {
        this->initComponents();
        this->connectPorts();
    }

    Tester ::
        ~Tester(void)
    {
    }

    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    void Tester ::
        singleChannel(void)
    {
        // check initial double buffer pointer state
        ASSERT_EQ(0,this->component.m_activeBuffer);
        // send a single channel value
        this->sendBuff(10,11);
        // call scheduler to transmit channel
        this->invoke_to_Run(0,0);
        this->component.doDispatch();
        // check to see if output port was called 
        ASSERT_FROM_PORT_HISTORY_SIZE(1);
        ASSERT_from_PktSend_SIZE(1);
        // build comparison output packet
        Fw::ComBuffer compare;
        // add packet descriptor
        ASSERT_EQ(Fw::FW_SERIALIZE_OK,compare.serialize(static_cast<FwPacketDescriptorType>(Fw::ComPacket::FW_PACKET_TELEM)));
        // add ID
        ASSERT_EQ(Fw::FW_SERIALIZE_OK,compare.serialize((U32)10));
        // add time
        Fw::Time checkTime(25,24);
        ASSERT_EQ(Fw::FW_SERIALIZE_OK,compare.serialize(checkTime));
        // add channel value
        ASSERT_EQ(Fw::FW_SERIALIZE_OK,compare.serialize((U32)11));
        // check returned packet
        ASSERT_from_PktSend(0,compare,0);

    }

    // ----------------------------------------------------------------------
    // Handlers for typed from ports
    // ----------------------------------------------------------------------

    void Tester ::
        from_PktSend_handler(
            const NATIVE_INT_TYPE portNum,
            Fw::ComBuffer &data,
            U32 context)
    {
        this->pushFromPortEntry_PktSend(data, context);
    }

    void Tester ::
        from_pingOut_handler(
            const NATIVE_INT_TYPE portNum,
            U32 key)
    {
        this->pushFromPortEntry_pingOut(key);
    }

    // ----------------------------------------------------------------------
    // Helper methods
    // ----------------------------------------------------------------------

    void Tester::sendBuff(FwChanIdType id, U32 val)
    {

        Fw::TlmBuffer buff;
        Fw::TlmBuffer readBack;
        Fw::SerializeStatus stat;
        Fw::Time timeTag(25,24);
        U32 retestVal;

        // create telemetry item
        buff.resetSer();
        stat = buff.serialize(val);
        ASSERT_EQ(Fw::FW_SERIALIZE_OK, stat);

        static bool tlc001 = false;

        if (not tlc001)
        {
            REQUIREMENT("TLC-001");
            tlc001 = true;
        }

        this->invoke_to_TlmRecv(0, id, timeTag, buff);
        // Read back value
        static bool tlc002 = false;

        if (not tlc002)
        {
            REQUIREMENT("TLC-002");
            tlc002 = true;
        }

        this->invoke_to_TlmGet(0, id, timeTag, readBack);
        // deserialize value
        retestVal = 0;
        readBack.deserialize(retestVal);
        ASSERT_EQ(retestVal, val);
    }

    void Tester ::
        connectPorts(void)
    {

        // TlmRecv
        this->connect_to_TlmRecv(
            0,
            this->component.get_TlmRecv_InputPort(0));

        // TlmGet
        this->connect_to_TlmGet(
            0,
            this->component.get_TlmGet_InputPort(0));

        // Run
        this->connect_to_Run(
            0,
            this->component.get_Run_InputPort(0));

        // pingIn
        this->connect_to_pingIn(
            0,
            this->component.get_pingIn_InputPort(0));

        // PktSend
        this->component.set_PktSend_OutputPort(
            0,
            this->get_from_PktSend(0));

        // pingOut
        this->component.set_pingOut_OutputPort(
            0,
            this->get_from_pingOut(0));
    }

    void Tester ::
        initComponents(void)
    {
        this->init();
        this->component.init(
            QUEUE_DEPTH, INSTANCE);
    }

} // end namespace Svc
