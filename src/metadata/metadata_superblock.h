/*
 * Copyright(c) 2012-2018 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef __METADATA_SUPERBLOCK_H__
#define __METADATA_SUPERBLOCK_H__

#define CACHE_MAGIC_NUMBER	0x187E1CA6

/**
 * @brief OCF cache metadata configuration superblock
 */
struct ocf_superblock_config {
	/** WARNING: Metadata probe disregards metadata version when
	 * checking if the cache is dirty - position of next two fields
	 * shouldn't change!! */
	uint8_t clean_shutdown;
	uint8_t dirty_flushed;
	uint32_t magic_number;

	uint32_t metadata_version;

	/* Currently set cache mode */
	ocf_cache_mode_t cache_mode;

	ocf_cache_line_t cachelines;
	uint32_t valid_parts_no;

	ocf_cache_line_size_t line_size;
	ocf_metadata_layout_t metadata_layout;
	uint32_t core_obj_count;

	unsigned long valid_object_bitmap[(OCF_CORE_MAX /
			(sizeof(unsigned long) * 8)) + 1];

	ocf_cleaning_t cleaning_policy_type;
	struct cleaning_policy_config cleaning[CLEANING_POLICY_TYPE_MAX];

	ocf_eviction_t eviction_policy_type;

	/* Current core sequence number */
	ocf_core_id_t curr_core_seq_no;

	struct ocf_user_part_config user_parts[OCF_IO_CLASS_MAX + 1];

	/*
	 * Checksum for each metadata region.
	 * This field has to be the last one!
	 */
	uint32_t checksum[metadata_segment_max];
};

/**
 * @brief OCF cache metadata runtime superblock
 */
struct ocf_superblock_runtime {
	struct ocf_part freelist_part;

	struct ocf_user_part_runtime user_parts[OCF_IO_CLASS_MAX + 1];

	uint32_t cleaning_thread_access;
};

static inline int ocf_metadata_set_shutdown_status(
		struct ocf_cache *cache,
		enum ocf_metadata_shutdown_status shutdown_status)
{
	return cache->metadata.iface.set_shutdown_status(cache,
			shutdown_status);
}

static inline int ocf_metadata_load_superblock(struct ocf_cache *cache)
{
	return cache->metadata.iface.load_superblock(cache);
}

static inline
int ocf_metadata_flush_superblock(struct ocf_cache *cache)
{
	if (cache->device)
		return cache->metadata.iface.flush_superblock(cache);

	return 0;
}

static inline uint64_t ocf_metadata_get_reserved_lba(
		struct ocf_cache *cache)
{
	return cache->metadata.iface.get_reserved_lba(cache);
}

#endif /* METADATA_SUPERBLOCK_H_ */
