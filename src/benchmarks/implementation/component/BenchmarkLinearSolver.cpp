/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the BenchmarkLinearSolver class.
 */

#include "BenchmarkLinearSolver.h"
#include "SparseMatrixCOO.h"
#include "SparseMatrixCSR.h"

#include "tt_interface_c.h"

#include <cstdlib>

#include <iostream>

namespace cupcfd
{
	namespace benchmark
	{
		template <class C, class I, class T>
		BenchmarkLinearSolver<C,I,T>::BenchmarkLinearSolver(std::string benchmarkName, I repetitions,
														  std::shared_ptr<cupcfd::data_structures::SparseMatrix<C,I,T>> matrixPtr,
														  std::shared_ptr<std::vector<T>> rhsVectorPtr,
														  std::shared_ptr<std::vector<T>> solVectorPtr,
														  std::shared_ptr<cupcfd::linearsolvers::LinearSolverInterface<C,I,T>> solverSystemPtr)
		: Benchmark<I,T>(benchmarkName, repetitions),
		  matrixPtr(matrixPtr),
		  rhsVectorPtr(rhsVectorPtr),
		  solVectorPtr(solVectorPtr),
		  solverSystemPtr(solverSystemPtr)
		{

		}

		template <class C, class I, class T>
		BenchmarkLinearSolver<C,I,T>::~BenchmarkLinearSolver() {
			// Shared Pointer will cleanup after itself as object is destroyed
		}

		template <class C, class I, class T>
		void BenchmarkLinearSolver<C,I,T>::setupBenchmark() {
			// Nothing to do here currently
		}

		template <class C, class I, class T>
		void BenchmarkLinearSolver<C,I,T>::recordParameters() {

		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkLinearSolver<C,I,T>::runBenchmark() {
			cupcfd::error::eCodes status;

			// Get the non-zero rows assigned to this rank
			I * rowIndexes;
			I nRowIndexes;
			status = matrixPtr->getNonZeroRowIndexes(&rowIndexes, &nRowIndexes);
			CHECK_ECODE(status)

			// Start tracking parameters/time for this block
			this->startBenchmarkBlock(this->benchmarkName);
			TreeTimerLogParameterInt("Repetitions", this->repetitions);

			this->recordParameters();

			for(I i = 0; i < this->repetitions; i++) {
				// Clear Matrix A
				TreeTimerEnterLibraryCall("ClearMatrixA");
				status = this->solverSystemPtr->clearMatrixA();
				TreeTimerExit("ClearMatrixA");
				CHECK_ECODE(status)

				// Clear Vector X
				TreeTimerEnterLibraryCall("ClearVectorX");
				status = this->solverSystemPtr->clearVectorX();
				TreeTimerExit("ClearVectorX");
				CHECK_ECODE(status)

				// Clear Vector B
				TreeTimerEnterLibraryCall("ClearVectorB");
				status = this->solverSystemPtr->clearVectorB();
				TreeTimerExit("ClearVectorB");
				CHECK_ECODE(status)

				// Set Vector B Values
				TreeTimerEnterLibraryCall("SetValuesVectorB");
				status = this->solverSystemPtr->setValuesVectorB(&((*(this->rhsVectorPtr.get()))[0]), this->rhsVectorPtr->size(), rowIndexes, nRowIndexes, matrixPtr->baseIndex);
				TreeTimerExit("SetValuesVectorB");
				CHECK_ECODE(status)

				// Set Vector X Values
				TreeTimerEnterLibraryCall("SetValuesVectorX");
				status = this->solverSystemPtr->setValuesVectorX(&((*(this->solVectorPtr.get()))[0]), this->solVectorPtr->size(), rowIndexes, nRowIndexes, matrixPtr->baseIndex);
				TreeTimerExit("SetValuesVectorX");
				CHECK_ECODE(status)

				// Set Matrix A Values/Build Matrix A (Structures already set inside setup)
				// Matrix must have same structures as one used during setup
				TreeTimerEnterLibraryCall("SetValuesMatrixA");
				status = this->solverSystemPtr->setValuesMatrixA(*matrixPtr);
				TreeTimerExit("SetValuesMatrixA");
				CHECK_ECODE(status)

				// Run Linear Solver
				TreeTimerEnterLibraryCall("Solve");
				status = this->solverSystemPtr->solve();
				TreeTimerExit("Solve");
				CHECK_ECODE(status)
			}

			// Stop tracking parameters/time for this block
			this->stopBenchmarkBlock(this->benchmarkName);

			// Cleanup
			free(rowIndexes);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation

template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCOO<int,float>, int, float>;
template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCOO<int,double>, int, double>;

template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCSR<int,float>, int, float>;
template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCSR<int,double>, int, double>;
