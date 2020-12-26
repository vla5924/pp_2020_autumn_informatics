// Copyright 2020 Gusarova Daria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include "./alg_strongin.h"

const double PI = 3.141592653589793238463;

TEST(Parallel_Operations_MPI, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> func = [](double x) {
        return sin(x);
    };
    const double a = 0.0001;
    const double b = 2.0;
    if (rank == 0) {
        double t1 = MPI_Wtime();
        coords res = SequentalStrongin(func, a, b);
        double t2 = MPI_Wtime();
        double t1_p = MPI_Wtime();
        coords res_p = SequentalStrongin(func, a, b);
        double t2_p = MPI_Wtime();
        std::cout << "Sequential Time :" << t2 - t1 << std::endl;
        std::cout << "Parallel Time :" << t2_p - t1_p << std::endl;
        ASSERT_EQ(true, res.first < 0.1);
    }
}

TEST(Parallel_Operations_MPI, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> func = [](double x) {
        return cos(x);
    };
    const double a = 0.0001;
    const double b = PI * 2.0;
    const double optPoint = PI;
    if (rank == 0) {
        double t1 = MPI_Wtime();
        coords res = SequentalStrongin(func, a, b);
        double t2 = MPI_Wtime();
        std::cout << "Sequential Time :" << t2 - t1 << std::endl;
        ASSERT_EQ(true, abs(res.first - optPoint) < 0.1);
    }
}

TEST(Parallel_Operations_MPI, Test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> func = [](double x) {
        return 2 * sin(x);
    };
    const double a = 0.0001;
    const double b = 2.0;
    if (rank == 0) {
        double t1 = MPI_Wtime();
        coords res = SequentalStrongin(func, a, b);
        double t2 = MPI_Wtime();
        std::cout << "Sequential Time :" << t2 - t1 << std::endl;
        ASSERT_EQ(true, res.first < 0.1);
    }
}

TEST(Parallel_Operations_MPI, Test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)> func = [](double x) {
        return sin(x);
    };
    const double a = 0.0001;
    const double b = 2.0;
    const double optPoint = a;
    double t1 = MPI_Wtime();
    coords res = ParallelStrongin(func, a, b);
    if (rank == 0) {
        double t2 = MPI_Wtime();
        std::cout << "Parallel Time :" << t2 - t1 << std::endl;
        ASSERT_EQ(true, abs(res.first - optPoint) < 0.1);
    }
}


TEST(Parallel_Operations_MPI, Test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> func = [](double x) {
        return cos(x);
    };
    const double a = 0.0;
    const double b = PI * 2.0;
    const double optPoint = PI;
    double t1 = MPI_Wtime();
    coords res = ParallelStrongin(func, a, b);
    if (rank == 0) {
        double t2 = MPI_Wtime();
        std::cout << "Parallel Time :" << t2 - t1 << std::endl;
        ASSERT_EQ(true, abs(res.first - optPoint) < 0.1);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
