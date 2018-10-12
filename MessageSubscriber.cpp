#include "MessageSubscriber.h"

#define CLIENT_ADDR "ipc:///tmp/pubsub_client.ipc"

MessageSubscriber* MessageSubscriber::shareInstance() {
    static MessageSubscriber messageSubscriber;
    return &messageSubscriber;
}

MessageSubscriber::MessageSubscriber() {
    if ((subscriber_ = nn_socket(AF_SP, NN_SUB)) < 0) {
        fatal("nn_socket");
    }
    if (nn_connect(subscriber_, CLIENT_ADDR) < 0) {
        fatal("nn_connet");
    }
}

std::string MessageSubscriber::subscribe(const std::string topic,
                                         std::string data) {
    // while (1) {
    int sz_topic = topic.length();
    if (nn_setsockopt(subscriber_, NN_SUB, NN_SUB_SUBSCRIBE, topic.c_str(),
                      sz_topic) < 0) {
        fatal("nn_setsockopt");
    }
    char* buf = NULL;
    int bytes = nn_recv(subscriber_, &buf, NN_MSG, 0);
    data = buf;
    printf("data is : %s\n", data.c_str());
    if (bytes < 0) {
        fatal("nn_recv");
    }
    return data;
    //}
}

MessageSubscriber::~MessageSubscriber() {
    if (subscriber_) {
        nn_close(subscriber_);
    }
}
// using google::protobuf::util::TimeUtil;
// int main(const int argc, const char** argv) {
/*
std::string buf;
while (1) {
    buf = MessageSubscriber::shareInstance()->subscribe_str(argv[1], buf);
    printf("CLIENT: RECEIVED %s\n", buf.c_str());
    // sleep(1);
}
return 0;
*/
// return 0;
//}