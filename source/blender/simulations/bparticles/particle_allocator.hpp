#pragma once

#include "particle_set.hpp"
#include "particles_state.hpp"

namespace BParticles {

/**
 * This class allows allocating new blocks from different particle containers.
 * A single instance is not thread safe, but multiple allocator instances can
 * be used by multiple threads at the same time.
 * It might hand out the same block more than once until it is full.
 */
class ParticleAllocator {
 private:
  ParticlesState &m_state;
  SmallVector<ParticlesBlock *> m_non_full_cache;
  SmallVector<ParticlesBlock *> m_allocated_blocks;

 public:
  ParticleAllocator(ParticlesState &state);
  ParticleAllocator(ParticleAllocator &other) = delete;
  ParticleAllocator(ParticleAllocator &&other) = delete;

  /**
   * Access all blocks that have been allocated by this allocator.
   */
  ArrayRef<ParticlesBlock *> allocated_blocks();

  ParticleSets request(StringRef particle_type_name, uint size);

  ParticlesState &particles_state();

 private:
  /**
   * Return a block that can hold new particles. It might create an entirely new one or use a
   * cached block.
   */
  ParticlesBlock &get_non_full_block(StringRef particle_type_name);

  /**
   * Allocate space for a given number of new particles. The attribute buffers might be distributed
   * over multiple blocks.
   */
  void allocate_block_ranges(StringRef particle_type_name,
                             uint size,
                             SmallVector<ParticlesBlock *> &r_blocks,
                             SmallVector<Range<uint>> &r_ranges);

  AttributesInfo &attributes_info(StringRef particle_type_name);
};

/* ParticleAllocator inline functions
 ********************************************/

inline ParticlesState &ParticleAllocator::particles_state()
{
  return m_state;
}

inline ArrayRef<ParticlesBlock *> ParticleAllocator::allocated_blocks()
{
  return m_allocated_blocks;
}

}  // namespace BParticles
