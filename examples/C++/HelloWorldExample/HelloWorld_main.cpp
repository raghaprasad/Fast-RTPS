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

/**
 * @file HelloWorld_main.cpp
 *
 */

#include "HelloWorldPublisher.h"
#include "HelloWorldSubscriber.h"

#include <fastrtps/Domain.h>

#include <fastrtps/utils/eClock.h>
#include <fastrtps/log/Log.h>
#include <fastrtps/utils/eClock.h>

#include <thread>

using namespace eprosima;
using namespace fastrtps;
using namespace rtps;

void doSomething() {
                    for (int i = 0; i < 60; i++) {
                        eClock::my_sleep(1000);
                    }
                }

int main(int argc, char** argv)
{
    std::cout << "Starting "<< std::endl;
    int type = 1;
    int count = 60;
    long sleep = 1000;
    int subs = 1;
    bool reuseParticipant = false;
    if(argc > 1)
    {
        if(strcmp(argv[1],"publisher")==0)
        {
            type = 1;
            if (argc >= 3)
            {
                // count = atoi(argv[2]);
                subs = atoi(argv[2]);
                // if (argc == 4)
                // {
                //     sleep = atoi(argv[3]);
                // }
                if (argc == 4) {
                    reuseParticipant = strcmp(argv[1],"true")==0;
                }
            }
        }
        else if(strcmp(argv[1],"subscriber")==0)
            type = 2;
    }
    else
    {
        std::cout << "publisher OR subscriber argument needed" << std::endl;
        Log::Reset();
        return 0;
    }

    switch(type)
    {
        case 1:
            {
                ParticipantAttributes PParam;
                PParam.rtps.builtin.use_SIMPLE_RTPSParticipantDiscoveryProtocol = true;
                PParam.rtps.builtin.use_SIMPLE_EndpointDiscoveryProtocol = true;
                PParam.rtps.builtin.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
                PParam.rtps.builtin.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
                PParam.rtps.builtin.domainId = 0;
                PParam.rtps.builtin.leaseDuration = c_TimeInfinite;
                PParam.rtps.setName("Participant_pub");
                eprosima::fastrtps::Participant* participant = Domain::createParticipant(PParam);
                HelloWorldPubSubType mtype;
                if (nullptr == participant) {
                    break;
                }
                

                std::thread t (doSomething); 
                t.join();
                // Domain::registerType(participant,&mtype);
                // std::cout << "Total  number of subs " << subs;

                // HelloWorldPublisher mypub;
                // if(mypub.init(participant))
                // {
                //     for (int i = 0 ; i < subs ; i ++) {
                //         HelloWorldSubscriber mysub;
                //         if (!reuseParticipant) {
                //             mysub.init();
                //         } else {
                //             mysub.init(participant);
                //         }
                //     }
                // }
                // if(mypub.init(participant))
                // {
                //     mypub.run(count, sleep);
                // }
                break;
            }
        case 2:
            {
                HelloWorldSubscriber mysub;
                if(mysub.init())
                {
                    mysub.run();
                }
                break;
            }
    }
    Domain::stopAll();
    Log::Reset();
    return 0;
}
