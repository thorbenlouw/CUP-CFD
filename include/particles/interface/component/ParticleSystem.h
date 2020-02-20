/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Contains declarations for the ParticleSystem class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_SYSTEM_INCLUDE_H
#define CUPCFD_PARTICLES_PARTICLE_SYSTEM_INCLUDE_H

#include "Error.h"
#include "UnstructuredMeshInterface.h"

#include <memory>

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Defines an interface for managing particles and particles emitters,
		 * including inter-rank communications.
		 *
		 * Design uses CRTP to permit the use of specialised Particle Systems/Emitters/Particles/Mesh
		 *
		 * The system tracks the position of particles and emitters, and performs atomic updates over
		 * a dt period cell-by-cell, updating cell and particle states.
		 *
		 * The interface is not intended for externally manipulating the particle characteristics
		 * directly (beyond a select few, such as travel time and activity) - rather it itself is responsible
		 * for ensuring their characteristics are correct. The impact of particles on the system is down
		 * to which specialisation is used, and what the method definitions for the particle behaviours are.
		 *
		 * The specialisations of this are primarily to permit different particle storage approaches/
		 * different exchange behaviours with a reuse of common particle system features (such as particle tracking)
		 * via the abstract interface
		 *
		 * Definitions:
		 * 'Active' particles are particles that exist and should continue to be processed on the current rank.
		 * 'Inactive' particles are particles that exist in storage, but should no longer be processed on the current
		 * rank (e.g. sent off-rank or particle no longer physically exists)
		 * 'Travelling' particles are particles that are both active and have some non-zero travel time assigned to them.
		 *
		 * @tparam S Specialisation type of the Particle System
		 * @tparam E Specialisation type of the Particle Emitters
		 * @tparam P Specialisation type of the Particles
		 * @tparam M Specialisation Type of the Mesh
		 * @tparam I Type of the indexing scheme
		 * @tparam T Type of the mesh spatial volume
		 * @tparam L Label type of the mesh
		 */
		template <class S, class E, class P, class M, class I, class T, class L>
		class ParticleSystem
		{
			public:
				// === Members ===

				/** Mesh **/
				// Meshes are too large to be simply copied (unlike emitters which are just small configurations)
				// so use a shared pointer instead.
				std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> mesh;

				// === Constructors/Deconstructors ===

				/**
				 * Empty Constructor
				 */
				ParticleSystem(std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> mesh);

				/**
				 * Deconstructor
				 */
				~ParticleSystem();

				// === Interface Methods ===

				/**
				 * Get the number of total number of particles stored in the system, both active and inactive.
				 */
				I getNParticles();

				/**
				 * Get the number of active particles in the system
				 */
				I getNActiveParticles();

				/**
				 * Get the number of active particles that have remaining travel time
				 */
				I getNTravelParticles();

				/**
				 * Add a particle emitter to the system.
				 *
				 * The cell ID of the emitter will be set/overwritten according to the position of the emitter
				 * in the mesh associated with this system
				 */
				inline cupcfd::error::eCodes addParticleEmitter(const E& emitter);

				/**
				 * Add a particle to the system (e.g. when loading from a data source, or receiving from
				 * another process).
				 *
				 * Since particles will be added far more frequently than emitters, the cellID of the particles
				 * will NOT be set by this function due to the significant overhead of searching through every cell
				 * in the mesh. They are expected to already be correct for their euclidean position
				 * and the mesh associated with this system. It is instead recommended to find an efficient means
				 * of sourcing this data prior to adding the particle(e.g. if from a data source,
				 * should also be stored alongside them, or if generated by an emitter set according to the
				 * cell ID of the emitter).
				 *
				 * @param particle The particle to add
				 */
				inline cupcfd::error::eCodes addParticle(const P& particle);

				/**
				 * Sets an active particle inactive, and updates the appropriate counters.
				 *
				 * The nature of the integer based identifier localID is left to the implementing system, but
				 * must uniquely identify an active particle (this could be an array index, an ID
				 * in a particle data structure etc).
				 */
				inline cupcfd::error::eCodes setParticleInactive(I particleID);

				/**
				 * Set the travel time of all active particles in the system to a specified value
				 */
				cupcfd::error::eCodes setActiveParticlesTravelTime(T travelTime);

				/**
				 *
				 */
				inline cupcfd::error::eCodes removeInactiveParticles();

				/**
				 * Exchange any particles that have gone off-node into ghost nodes, so they can be
				 * managed by their respective processes.
				 *
				 * This will require all ranks that are in the mesh graph's communicator to
				 * participate.
				 *
				 * Any particle sent to another rank should be marked as inactive by this process
				 */
				cupcfd::error::eCodes exchangeParticles();

				/**
				 * Generate and add all particles generated over a time delta change of dt
				 * to the system.
				 *
				 * Each of the particles generated will be assigned a travel time based on how
				 * much time is remaining in the dt period from the point at which they are generated
				 * (since not all particles are generated at the beginning).
				 *
				 * Note: Since the travel time of the emitted particles will differ, this step should be done
				 * *before* setActiveParticlesTravelTime if it is not intended to overwrite the values.
				 */
				cupcfd::error::eCodes generateEmitterParticles(T dt);

				/**
				 * Advance the system by dt units of time.
				 *
				 * This will generate any particles produced by emitters during this period, and move both existing
				 * and newly generated particles.
				 * Particles that already exist will have their travel time overwritten to be dt, those that are
				 * generated will use whatever time is remaining in dt depending on when in the time period they
				 * get created.
				 *
				 * Particles that have expired are made inaccessible/removed by the system
				 */
				cupcfd::error::eCodes updateSystem(T dt);

				/**
				 * Advance the system by an atomic update step, if there are any particles within
				 * the system that have remaining time to travel.
				 *
				 * The definition of what constitutes as 'atomic' can be left to the specialising system
				 * - e.g. it could be furst particle to exit a cell, all particles to advance one cell etc.
				 *
				 * This method does not add any travel time to particles, or generate any new particles.
				 */
				cupcfd::error::eCodes updateSystemAtomic();


		};
	}
}

// Include Header Level Definitions
#include "ParticleSystem.ipp"

#endif
