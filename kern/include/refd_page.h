/* Copyright (c) 2016 Google Inc.
 * Barret Rhoden <brho@cs.berkeley.edu>
 * See LICENSE for details.
 *
 * Helpers for reference counted pages.
 *
 * Some code wants to use reference counted pages.  I'd like to keep these
 * uses separate from the main memory allocator.  Code that wants reference
 * counted pages can use these helpers. */

#pragma once

#include <kref.h>
#include <page_alloc.h>
#include <pmap.h>
#include <kmalloc.h>
#include <assert.h>

struct refd_page {
	void			*rp_kva;
	struct kref		rp_kref;
};

static struct page *rp2page(struct refd_page *rp)
{
	return kva2page(rp->rp_kva);
}

static void refd_page_release(struct kref *kref)
{
	struct refd_page *rp = container_of(kref, struct refd_page, rp_kref);

	page_decref(rp2page(rp));
	kfree(rp);
}

static struct refd_page *get_refd_page(void *kva)
{
	struct refd_page *rp;

	if (!kva)
		return 0;
	rp = kmalloc(sizeof(struct refd_page), MEM_WAIT);
	assert(rp);
	rp->rp_kva = kva;
	kref_init(&rp->rp_kref, refd_page_release, 1);
	return rp;
}

static void refd_page_decref(struct refd_page *rp)
{
	kref_put(&rp->rp_kref);
}
