cmake_minimum_required(VERSION 3.15)
project(reading_room_v2 C)

set(CMAKE_C_STANDARD 11)

add_executable(reading_room_v2 reading_room_v2.c utils/definitions.h utils/message_queue.h utils/semaphore.h utils/shared_memory.h utils/titles.h utils.h)
add_executable(person_v2 person_v2.c utils/definitions.h utils/message_queue.h utils/semaphore.h utils/shared_memory.h utils/titles.h utils.h)
