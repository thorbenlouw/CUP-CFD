/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations at Header Level of Common Broadcast Operations for Parallel Communications
 */

#ifndef CUPCFD_COMM_BROADCAST_IPP_H
#define CUPCFD_COMM_BROADCAST_IPP_H

#include "BroadcastMPI.h"
#include "ArrayDrivers.h"

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		cupcfd::error::eCodes Broadcast(T * buf, int nBuf, int sourcePID, Communicator& myComm)
		{
			// Error Checks

			// Call a suitable underlying comm library driver (in this case, MPI)
			// Use nBuf for nEle to communicate full buffer.
			cupcfd::error::eCodes err = cupcfd::comm::mpi::BroadcastMPI(buf, nBuf, sourcePID, myComm.comm);

			// Passthrough error code
			return err;
		}

		template <class T>
		cupcfd::error::eCodes Broadcast(T * bSend, int nBSend, T * bRecv, int nBRecv, int sourcePID, Communicator& myComm)
		{
			cupcfd::error::eCodes err;

			if(myComm.rank == sourcePID)
			{
				err = cupcfd::comm::mpi::BroadcastMPI(bSend, nBSend, sourcePID, myComm.comm);
				//For consistency we copy the src buffer to the recv buffer, but this is technically an imbalance - not sure this behaviour is desirable
				//since it makes the function more expensive
				cupcfd::utility::drivers::copy(bSend, nBSend, bRecv, nBRecv);
			}
			else
			{
				err = cupcfd::comm::mpi::BroadcastMPI(bRecv, nBRecv, sourcePID, myComm.comm);
			}

			return err;
		}

		template <class T>
		cupcfd::error::eCodes Broadcast(T * bSend, int nBSend, T ** bRecv, int * nBRecv, int sourcePID, Communicator& myComm)
		{
			// Obtain the number of expected elements from the source process via a single element broadcast
			Broadcast(&nBSend, 1, nBRecv, 1, sourcePID, myComm);

			// Allocate the space
			*bRecv = (T *) malloc(sizeof(T) * *nBRecv);

			cupcfd::error::eCodes err = Broadcast(bSend, nBSend, *bRecv, *nBRecv, sourcePID, myComm);

			// Cleanup
			// Note: Recv buffer is not free'd here - left to the caller.
			// No other cleanup currently

			return err;
		}
	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "Broadcast.ipp"

#endif
