#ifndef MSG_PUB_H_
#define MSG_PUB_H_
#include <google/protobuf/message.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/reqrep.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include "fatal.h"

class MessagePublisher {
   public:
    static MessagePublisher *shareInstance();

    void publish(const std::string topic, google::protobuf::Message &message);
    void publish(const std::string topic, const std::string data);

    ~MessagePublisher();

   protected:
    MessagePublisher();
    int publisher_ = 0;
};

#endif