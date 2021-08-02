/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the Communicator class
 */

#include "Communicator.h"

#include <iostream>

#include "mpi.h"

namespace cupcfd::comm {
Communicator::Communicator() : comm(MPI_COMM_NULL) {
  this->rank = 0;
  this->size = 1;
  this->root_rank = 0;
  this->root = true;

  // Override the null comm
  int err = MPI_Comm_dup(MPI_COMM_SELF, &(this->comm));

  if (err != MPI_SUCCESS) {
    std::cout << "Warning: Error Duplicating MPIComm in MPI Communicator "
                 "Constructor\n";
  }
}

Communicator::Communicator(MPI_Comm mpiComm) : comm(MPI_COMM_NULL) {
  // Duplicate the communicator
  int err = MPI_Comm_dup(mpiComm, &(this->comm));

  if (err != MPI_SUCCESS) {
    std::cout << "Warning: Error Duplicating MPIComm in MPI Communicator "
                 "Constructor\n";
  }

  // Store basic properties
  MPI_Comm_rank(this->comm, &(this->rank));
  MPI_Comm_size(this->comm, &(this->size));

  // Leave root rank as always zero for now
  this->root_rank = 0;

  // Determine if this process is root
  if (this->rank == this->root_rank) {
    this->root = true;
  } else {
    this->root = false;
  }
}

Communicator::Communicator(Communicator& comm) : comm(MPI_COMM_NULL) {
  // Use the copy operator
  *this = std::move(comm);
}

Communicator::~Communicator() {
  // === Cleanup ===

  // Stored MPI Comms are all duplicated from other MPI Communicators.
  // They should be released here else we will lose track of the duplicates.
  MPI_Comm_free(&(this->comm));
}

Communicator* Communicator::clone() { return new Communicator(*this); }

}  // namespace cupcfd::comm
