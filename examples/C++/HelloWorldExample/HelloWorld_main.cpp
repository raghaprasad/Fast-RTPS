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
        // if(strcmp(argv[1],"publisher")==0)
        // {
        //     type = 1;
        //     if (argc >= 3)
        //     {
        //         // count = atoi(argv[2]);
        //         subs = atoi(argv[2]);
        //         // if (argc == 4)
        //         // {
        //         //     sleep = atoi(argv[3]);
        //         // }
        //         if (argc == 4) {
        //             reuseParticipant = strcmp(argv[1],"true")==0;
        //         }
        //     }
        // }
        // else if(strcmp(argv[1],"subscriber")==0)
        //     type = 2;
        if (strcmp(argv[1], "1")==0) {
            std::cout << "Creating only participant " << std::endl;
            type = 1;
        }
        else if (strcmp(argv[1], "2")==0) {
            std::cout << "Creating only 1 participant and 1 publisher " << std::endl;
            type = 2;
        }
        else if (strcmp(argv[1], "3")==0) {
            std::cout << "Creating only 1 participant and 1 publisher and 1 subscriber" << std::endl;
            type = 3;
        }

        if (argc >=3) {
            reuseParticipant = strcmp(argv[2],"true")==0;
            
            if (argc == 4) {
                subs = atoi(argv[3]);
            }
        }
    }
    else
    {
        std::cout << "publisher OR subscriber argument needed" << std::endl;
        Log::Reset();
        return 0;
    }

    ParticipantAttributes PParam;
    PParam.rtps.builtin.use_SIMPLE_RTPSParticipantDiscoveryProtocol = true;
    PParam.rtps.builtin.use_SIMPLE_EndpointDiscoveryProtocol = true;
    PParam.rtps.builtin.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
    PParam.rtps.builtin.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
    PParam.rtps.builtin.domainId = 0;
    PParam.rtps.builtin.leaseDuration = c_TimeInfinite;
    PParam.rtps.setName("Participant_pub");

    switch(type)
    {
        case 1: // only participant
            {
                eprosima::fastrtps::Participant* participant = Domain::createParticipant(PParam);
                // eprosima::fastrtps::Participant* n_participant = Domain::createParticipant(PParam);
                if (nullptr == participant) {
                    break;
                }
                std::thread t (doSomething); 
                t.join();
                // HelloWorldPubSubType mtype;
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
                eprosima::fastrtps::Participant* participant = Domain::createParticipant(PParam);
                if (nullptr == participant) {
                    break;
                }
                HelloWorldPubSubType mtype;
                Domain::registerType(participant,&mtype);
                // std::cout << "Total  number of subs " << subs;

                HelloWorldPublisher mypub;
                if(mypub.init(participant)) {
                    mypub.run(count, sleep);
                }
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
        case 3:
            {
                eprosima::fastrtps::Participant* participant = Domain::createParticipant(PParam);
                if (nullptr == participant) {
                    break;
                }
                HelloWorldPubSubType mtype;
                Domain::registerType(participant,&mtype);
                std::cout << "Total  number of subs " << subs << std::endl;
                HelloWorldPublisher mypub;
                std::vector<std::thread> threadPool; 
                if(mypub.init(participant)) {
                    std::thread thread(&HelloWorldPublisher::runThread, &mypub, count, sleep);
                    for (int i = 0 ; i < subs ; i ++) {
                        // memory leak -- but thats ok 
                        threadPool.emplace_back(&HelloWorldSubscriber::initm, new HelloWorldSubscriber(), participant);
                    }
                    for (int i = 0 ; i < threadPool.size() ; i ++) {
                        threadPool[i].join();
                    }
                    thread.join();
                }
                break;
            }
        case 4:
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
