trocc

TPM RPC Open/Close Context

This client is used to demonstrate the way to do RPC from local machine to the remote TPM. Open/Close context are the most basic functions used by all the TPM commands. Based on the framework of trocc, other RPC functions could be extended accordingly.

NOTE

1. All the header files are ported from trousers-0.3.11

2. The RPC server by default should be tcsd or any other TPM daemon which accepts TCSD packet

3. The logs from client/server are using hacked version of libtspi and tcsd

4. This program needs the support of trousers API

BUILD

gcc -o rpc_oc rpc_oc.c -DHAVE_PTHREAD_H -ltspi -lpthread

RUN

./rpc_oc

Sep 2, 2013

root@davejingtian.org

http://davejingtian.org
