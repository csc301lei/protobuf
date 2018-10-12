#include "MessagePublisher.h"

#define ROUTER_ADDR "tcp://127.0.0.1:19001"

MessagePublisher *MessagePublisher::shareInstance() {
    static MessagePublisher messagePublisher;
    return &messagePublisher;
}

MessagePublisher::MessagePublisher() {
    if ((publisher_ = nn_socket(AF_SP, NN_PUB)) < 0) {
        fatal("nn_socket");
    }
    if (nn_connect(publisher_, ROUTER_ADDR) < 0) {
        fatal("nn_connect");
    }
}

MessagePublisher::~MessagePublisher() {
    if (publisher_) {
        nn_close(publisher_);
    }
}

void MessagePublisher::publish(const std::string topic,
                               google::protobuf::Message &message) {
    char buf[4096] = "";
    sprintf(buf, "%s%c", topic.c_str(), '|');
    message.SerializeToArray(buf + topic.length() + 1,
                             4096 - topic.length() - 1);
    int len = topic.length() + message.ByteSize() + 1;
    // printf("sz_d is %d, msg.ByteSize() is %d, len is %d.\n", sz_d,
    // msg.ByteSize(), len); printf("SERVER: PUBLISHING %s\n",buf);
    int bytes = nn_send(publisher_, buf, len, 0);
    if (bytes < 0) {
        fatal("nn_send");
    }
}

void MessagePublisher::publish(const std::string topic,
                               const std::string data) {
    char buf[4096] = "";
    sprintf(buf, "%s%c%s", topic.c_str(), '|', data.c_str());
    int len = topic.length() + data.length() + 1;
    // printf("SERVER: PUBLISHING %s\n", buf);
    int bytes = nn_send(publisher_, buf, len, 0);
    if (bytes < 0) {
        fatal("nn_send");
    }
}

/*
bool if_named_topic(const std::string topic)
{
    std::string buf = NULL;
    int sock;
    int rv;
    int bytes;
    int sz_topic = topic.length() + 1;
    if ((sock = nn_socket(AF_SP, NN_REQ)) < 0)
    {
        fatal("nn_socket");
    }
    if ((rv = nn_connect(sock, CHECK_NAME)) < 0)
    {
        fatal("nn_connect");
    }
    if ((bytes = nn_send(sock, topic.c_str(), sz_topic, 0)) < 0)
    {
        fatal("nn_send");
    }
    if ((bytes = nn_recv(sock, &buf, NN_MSG, 0)) < 0)
    {
        fatal("nn_recv");
    }

    if (buf == "new")
    {
        return false;
    }
    if (buf == "exist")
    {
        return true;
    }
}


int msg_pub(const std::string topic, google::protobuf::Message &message)
{
    //if( if_named_topic(d) )
    //{
    //    printf("Topic name exist! Please rename your topic or longer it…\n");
    //    return 0;
    //}

    int sock;
    if ((sock = nn_socket(AF_SP, NN_PUB)) < 0)
    {
        fatal("nn_socket");
    }
    if (nn_connect(sock, ROUTER_ADDR) < 0)
    {
        fatal("nn_connect");
    }

    char buf[4096] = "";
    int sz_topic = topic.length();
    sprintf(buf, "%s%c", topic, '|');
    message.SerializeToArray(buf + sz_topic + 1, 4096 - sz_topic - 1);
    int len = sz_topic + message.ByteSize() + 1;
    //printf("sz_d is %d, msg.ByteSize() is %d, len is %d.\n", sz_d,
msg.ByteSize(), len);
    //printf("SERVER: PUBLISHING %s\n",buf);
    int bytes = nn_send(sock, buf, len, 0);
    if (bytes < 0)
    {
        fatal("nn_send");
    }
    //sleep(1);

    nn_close(sock);
}

int msg_pub(const std::string topic, const std::string data)
{
    //if( if_named_topic(topic) )
    //{
    //    printf("Topic name exist! Please rename your topic or longer it…\n");
    //    return 0;
    //}
    int sock;
    if ((sock = nn_socket(AF_SP, NN_PUB)) < 0)
    {
        fatal("nn_socket");
    }
    if (nn_connect(sock, ROUTER_ADDR) < 0)
    {
        fatal("nn_connect");
    }

    int sz_topic = topic.length() + 1;
    int sz_data = data.length(); // '\0' too
    int len = sz_topic + sz_data + 1;
    char buf[100];
    sprintf(buf, "%s%c%s", topic, '|', data);
    //printf("SERVER: PUBLISHING %s\n", buf);
    int bytes = nn_send(sock, buf, len, 0);
    if (bytes < 0)
    {
        fatal("nn_send");
    }
    //sleep(1);

    nn_close(sock);
}
*/
