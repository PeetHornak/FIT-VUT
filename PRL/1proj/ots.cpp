//
// Created by xhorna14 on 2020-03-08.
//

#include <mpi.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <chrono>

#define TAG 0
//#define DEBUG
//#define CHRONO

void do_phase(int phase, int proc_num, int my_id, int* current_number, MPI_Status *mpi_stat){
    int recvd_number;
    if (my_id % 2 != phase && my_id != 0) {
        MPI_Send(current_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD);
        MPI_Recv(current_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD, mpi_stat);
    } else if (my_id % 2 == phase && my_id < proc_num - 1){
        MPI_Recv(&recvd_number, 1, MPI_INT, my_id + 1, TAG, MPI_COMM_WORLD, mpi_stat);
        if (*current_number > recvd_number) {
            MPI_Send(current_number, 1, MPI_INT, my_id + 1, TAG, MPI_COMM_WORLD);
            #ifdef  DEBUG
            std::cout << my_id << "|-> " << *current_number << " |" << my_id +1<< std::endl;
            std::cout << my_id << "|  " << recvd_number << " <-|" << my_id +1<< std::endl;
            #endif
            *current_number = recvd_number;
        }
        else {
            MPI_Send(&recvd_number, 1, MPI_INT, my_id + 1, TAG, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char *argv[]) {
    int proc_num;
    int my_id;
    int current_number;
    std::vector<int> *numbers = nullptr;
    std::ifstream file("numbers", std::ios::in | std::ios::binary);
    MPI_Status mpi_stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    #ifdef CHRONO
    auto t0 = std::chrono::high_resolution_clock::now();
    #endif

    // Master reads input from stdin
    if(my_id == 0 ) {
        numbers = new std::vector<int> ((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // Send input_numbers to all processes
        for (int i = 0; i < numbers->size(); i++) {
            MPI_Send(&((*numbers)[i]), 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }
    }
    // Each process receives number from master
    MPI_Recv(&current_number, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &mpi_stat);

    // Sort
    for (int i = 0; i <= (proc_num / 2); i++) {
        // Odd phase
        do_phase(0, proc_num, my_id, &current_number, &mpi_stat);
        // Even phase
        do_phase(1, proc_num, my_id, &current_number, &mpi_stat);

    }

    std::vector<int> sorted_numbers;
    for (int i = 1; i < proc_num; i++) {
        // Each slave process sends his number to master
        if (my_id == i){
            MPI_Send(&current_number, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        }
        // Master receives all numbers and saves them to vector
        if (my_id == 0) {
            int final_recv;
            MPI_Recv(&final_recv, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &mpi_stat);
            sorted_numbers.push_back(final_recv);
        }
    }

    #ifdef CHRONO
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = t1 - t0;
    std::cout << duration.count() << std::endl;
    #endif

    // Print sorted numbers to stdout
    if (my_id == 0) {
        for (int i = 0; i < numbers->size(); i++) {
            std::cout << (*numbers)[i] << " ";
        }
        // First print master number
        std::cout << std::endl << current_number << std::endl;
        for (int i = 0; i < sorted_numbers.size(); i++){
            std::cout << sorted_numbers[i] << std::endl;
        }
    }
    MPI_Finalize();

    return 0;
}
