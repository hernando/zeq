
/* Copyright (c) 2014, Human Brain Project
 *                     Stefan.Eilemann@epfl.ch
 */

#define BOOST_TEST_MODULE zeq_receiver

#include "broker.h"

#include <chrono>

bool gotOne = false;
bool gotTwo = false;

void onEvent1( const zeq::Event& ) { gotOne = true; }
void onEvent2( const zeq::Event& ) { gotTwo = true; }

void testReceive( zeq::Publisher& publisher, zeq::Receiver& receiver,
                  bool& var1, bool& var2, const int line )
{
    using zeq::vocabulary::serializeEcho;
    gotOne = false;
    gotTwo = false;

    const auto startTime = std::chrono::high_resolution_clock::now();
    for( ;; )
    {
        BOOST_CHECK( publisher.publish( serializeEcho( test::echoMessage )));
        while( receiver.receive( 100 )) {}

        if( var1 && var2 )
            break;

        const auto endTime = std::chrono::high_resolution_clock::now();
        const auto elapsed =
            std::chrono::nanoseconds( endTime - startTime ).count() / 1000000;
        if( elapsed > 2000 /*ms*/ )
            break;
    }
    BOOST_CHECK_MESSAGE( var1, (&var1 == &gotOne ? "Event 1" : "Event 2") <<
                               " not received (l." << line << ")" );
    if( &var1 != &var2 )
        BOOST_CHECK_MESSAGE( var2, (&var2 == &gotOne ? "Event 1" : "Event 2") <<
                                   " not received (l." << line << ")" );
}
void testReceive( zeq::Publisher& publisher, zeq::Receiver& receiver,
                  bool& var, const int line )
{
    testReceive( publisher, receiver, var, var, line );
}

BOOST_AUTO_TEST_CASE(test_two_subscribers)
{
    zeq::Publisher publisher( zeq::NULL_SESSION );
    zeq::Subscriber subscriber1( test::buildURI( "localhost", publisher ));
    zeq::Subscriber subscriber2( test::buildURI( "localhost", publisher ),
                                 subscriber1 );

    BOOST_CHECK( subscriber1.registerHandler( zeq::vocabulary::EVENT_ECHO,
                                std::bind( &onEvent1, std::placeholders::_1 )));
    BOOST_CHECK( subscriber2.registerHandler( zeq::vocabulary::EVENT_ECHO,
                                std::bind( &onEvent2, std::placeholders::_1 )));

    testReceive( publisher, subscriber1, gotOne, gotTwo, __LINE__ );
    testReceive( publisher, subscriber2, gotOne, gotTwo, __LINE__ );
}

BOOST_AUTO_TEST_CASE(test_publisher_routing)
{
    zeq::Publisher publisher( zeq::NULL_SESSION );
    zeq::Publisher silentPublisher( zeq::NULL_SESSION );
    zeq::Subscriber* subscriber1 =
        new zeq::Subscriber( test::buildURI( "localhost", silentPublisher ));
    zeq::Subscriber subscriber2( test::buildURI( "localhost", publisher ),
                                 *subscriber1 );

    BOOST_CHECK( subscriber1->registerHandler( zeq::vocabulary::EVENT_ECHO,
                                std::bind( &onEvent1, std::placeholders::_1 )));
    BOOST_CHECK( subscriber2.registerHandler( zeq::vocabulary::EVENT_ECHO,
                                std::bind( &onEvent2, std::placeholders::_1 )));

    testReceive( publisher, *subscriber1, gotTwo, __LINE__ );
    BOOST_CHECK( !gotOne );

    testReceive( publisher, subscriber2, gotTwo, __LINE__ );
    BOOST_CHECK( !gotOne );

    delete subscriber1;
    testReceive( publisher, subscriber2, gotTwo, __LINE__ );
    BOOST_CHECK( !gotOne );
}
