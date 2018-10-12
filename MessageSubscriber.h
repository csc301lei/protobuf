#ifndef MSG_SUB_H_
#define MSG_SUB_H_

#include <google/protobuf/message.h>
#include <google/protobuf/util/time_util.h>
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
#include <functional>
#include <iostream>
#include <thread>
#include "addressbook.pb.h"
#include "fatal.h"

#define CLIENT_ADDR "ipc:///tmp/pubsub_client.ipc"

class MessageSubscriber {
   public:
    static MessageSubscriber *shareInstance();
    std::string subscribe(const std::string topic, std::string data);
    // int Subscribe(std::function<void(char*)> cb) {
    //   m_cb_ = cb;
    //    if (m_cb_) {
    //        m_cb_(str);
    //   }
    //}

    ~MessageSubscriber();

   private:
    MessageSubscriber();
    int subscriber_ = 0;
    // std::function<void(char*)> m_cb_;
    // std::string str;
};

template <typename U>
class MessageSubscriber_ {
   public:
    static MessageSubscriber_ *shareInstance() {
        static MessageSubscriber_ messageSubscriber;
        return &messageSubscriber;
    }

    U subscribe(const std::string topic, U &message) {
        int sock;
        if ((sock = nn_socket(AF_SP, NN_SUB)) < 0) {
            fatal("nn_socket");
        }

        int sz_topic = topic.length();
        if (nn_setsockopt(sock, NN_SUB, NN_SUB_SUBSCRIBE, topic.c_str(),
                          sz_topic) < 0) {
            fatal("nn_setsockopt");
        }
        if (nn_connect(sock, CLIENT_ADDR) < 0) {
            fatal("nn_connet");
        }
        char *buf = NULL;
        int bytes = nn_recv(sock, &buf, NN_MSG, 0);
        if (bytes < 0) {
            fatal("nn_recv");
        }
        // printf("CLIENT: RECEIVED %s\n", buf);

        int i = 0;
        int j = 0;
        for (i = 0; i < bytes; i++) {
            if (buf[i] == '|') {
                j = i + 1;
                break;
            }
        }
        message.ParseFromArray(buf + j, bytes - j);
        // printf("CLIENT: RECEIVED ok\n");
        nn_freemsg(buf);
        return message;
    }

    ~MessageSubscriber_() {
        if (subscriber_) {
            nn_close(subscriber_);
        }
    }

   private:
    MessageSubscriber_() {
        if ((subscriber_ = nn_socket(AF_SP, NN_SUB)) < 0) {
            fatal("nn_socket");
        }
        if (nn_connect(subscriber_, CLIENT_ADDR) < 0) {
            fatal("nn_connet");
        }
    }
    int subscriber_ = 0;
};
#endif