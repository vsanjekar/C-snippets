all:
	protoc-c --c_out=. message.proto
	gcc -g proto_test.c message.pb-c.c -o proto -I/usr/local/include -L/usr/local/lib -lprotobuf-c

clean:
	rm message.pb-c.c message.pb-c.h
	rm proto
