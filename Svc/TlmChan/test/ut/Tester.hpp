// ======================================================================
// \title  TlmChan/test/ut/Tester.hpp
// \author tcanham
// \brief  hpp file for TlmChan test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Svc/TlmChan/TlmChanImpl.hpp"

namespace Svc
{

    class Tester : public TlmChanGTestBase
    {

        // ----------------------------------------------------------------------
        // Construction and destruction
        // ----------------------------------------------------------------------

    public:
        //! Construct object Tester
        //!
        Tester(void);

        //! Destroy object Tester
        //!
        ~Tester(void);

    public:
        // ----------------------------------------------------------------------
        // Tests
        // ----------------------------------------------------------------------

        //! Send a single channel
        //!
        void singleChannel(void);

    private:
        // ----------------------------------------------------------------------
        // Handlers for typed from ports
        // ----------------------------------------------------------------------

        //! Handler for from_PktSend
        //!
        void from_PktSend_handler(
            const NATIVE_INT_TYPE portNum, /*!< The port number*/
            Fw::ComBuffer &data,           /*!< Buffer containing packet data*/
            U32 context                    /*!< Call context value; meaning chosen by user*/
        );

        //! Handler for from_pingOut
        //!
        void from_pingOut_handler(
            const NATIVE_INT_TYPE portNum, /*!< The port number*/
            U32 key                        /*!< Value to return to pinger*/
        );

    private:
        // ----------------------------------------------------------------------
        // Helper methods
        // ----------------------------------------------------------------------

        void sendBuff(FwChanIdType id, U32 val);

        //! Connect ports
        //!
        void connectPorts(void);

        //! Initialize components
        //!
        void initComponents(void);

    private:
        // ----------------------------------------------------------------------
        // Variables
        // ----------------------------------------------------------------------

        //! The component under test
        //!
        TlmChanImpl component;
    };

} // end namespace Svc

#endif
