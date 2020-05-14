#include <mpi.h>
#include <limits>
#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>

//#define DEBUG

int main(int argc, char *argv[]) {
    int proc_num;
    int my_id;

    MPI_Status mpi_stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    std::string input;
    std::vector<int> numbers;
    // Read and parse input
    std::cin >> input;

    std::stringstream ss(input);
    for (int i; ss >> i;) {
        numbers.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }
    int numbers_size = numbers.size();
#ifdef DEBUG
    if (my_id == 0) {
        for (int c = 0; c < numbers_size; c++) {
            std::cout << numbers[c] << " ";
        }
        std::cout << std::endl;
    }
#endif

    int input_size = pow(2, (int)(log2(numbers_size) + 0.999));

    // Broadcast input size
    MPI_Bcast(&numbers_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&input_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    float arr[input_size];

    // Load arr
    if (my_id == 0) {
        for (int i = 0; i < numbers.size(); i++) {
            arr[i] = numbers[i];
        }
        for (int i = numbers.size(); i < input_size; i++) {
            arr[i] =  arr[0];
        }
    }
    // Broadcast input
    MPI_Bcast(arr, input_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Compute diff between first and other points
    for (int i = 0; i <= ((input_size - 1) / proc_num); i++) {
        int idx = i * proc_num + my_id;
        if (idx == 0) {
            continue;
        }
        arr[idx] = atan((arr[idx] - arr[0]) / idx);
    }

    // Each process broadcasts it's changed numbers to other processes
    for (int i = 0; i < input_size; i++) {
        MPI_Bcast(&arr[i], 1, MPI_FLOAT, i % proc_num, MPI_COMM_WORLD);
    }

    arr[0] = std::numeric_limits<float>::lowest();
#ifdef DEBUG
    if (my_id == 0) {
        for (int c = 0; c < input_size; c++) {
            std::cout << arr[c] << " ";
        }
        std::cout << std::endl;
    }
#endif
    // Reduce
    float max_prev_angle[input_size];
    std::copy(arr, arr + input_size, max_prev_angle);
    int p = 2;
    int log_count = log(input_size) / log(p);
    for (int d = 0; d < log_count; d++) {
        int power = pow(p, d + 1);
        for (int i = 0; i < input_size; i = i + proc_num * power) {
            int second_idx = ((my_id + 1) * power - 1) + i;
            int first_idx = second_idx - power / 2;
            if (first_idx < numbers_size) {
                max_prev_angle[second_idx] = std::max(max_prev_angle[second_idx], max_prev_angle[first_idx]); // MAX
            }
            // Each process broadcasts it's changed numbers to other processes
            for (int j = 0; j < proc_num; j++) {
                int s_idx = ((j + 1) * power - 1) + i;
                if (s_idx >= input_size)
                    break;
                MPI_Bcast(&max_prev_angle[s_idx], 1, MPI_FLOAT, j, MPI_COMM_WORLD);
            }
        }
#ifdef DEBUG
        if (my_id == 0) {
            for (int c = 0; c < input_size; c++) {
                std::cout << max_prev_angle[c] << " ";
            }
            std::cout << std::endl;
        }
#endif
    }

    max_prev_angle[input_size - 1] = std::numeric_limits<float>::lowest();;

#ifdef DEBUG
    if (my_id == 0) {
        std::cout << "END OF REDUCE" << std::endl;
    }
#endif

    for (int d = log_count - 1 ;d >= 0; d--) {
        int power = pow(p, d + 1);
        for (int i = 0; i < input_size; i = i + proc_num * power) {
            int second_idx = ((my_id + 1) * power - 1) + i;
            int first_idx = second_idx - power / 2;
            if (second_idx < input_size) {
                float t = max_prev_angle[first_idx];
                max_prev_angle[first_idx] = max_prev_angle[second_idx];
                max_prev_angle[second_idx] = std::max(max_prev_angle[second_idx], t); // MAX
            }
            for (int j = 0; j < proc_num; j++) {
                int s_idx = ((j + 1) * power - 1) + i;
                int f_idx = s_idx - power / 2;
                if (s_idx >= input_size)
                    break;
                MPI_Bcast(&max_prev_angle[s_idx], 1, MPI_FLOAT, j, MPI_COMM_WORLD);
                MPI_Bcast(&max_prev_angle[f_idx], 1, MPI_FLOAT, j, MPI_COMM_WORLD);
            }
        }
#ifdef DEBUG
        if (my_id == 0) {
            for (int c = 0; c < input_size; c++) {
                std::cout << max_prev_angle[c] << " ";
            }
            std::cout << std::endl;
        }
#endif
    }

    if (my_id == 0) {
        std::cout << "_";
        for (int c = 1; c < numbers_size; c++) {
            if (arr[c] > max_prev_angle[c])
                std::cout << ",v";
            else
                std::cout << ",u";
        }
        std::cout << std::endl;
    }
    MPI_Finalize();
    return 0;
}