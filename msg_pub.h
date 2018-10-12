#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/reqrep.h>
#include <nanomsg/pipeline.h>
#include <iostream>
#include <pthread.h>
#include <google/protobuf/message.h>
#include "fatal.h"

int msg_pub(const char *d);
int msg_pub(const char * d, google::protobuf::Message &msg);
int msg_pub(const char *topic, const char *d);