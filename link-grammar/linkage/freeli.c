/*************************************************************************/
/* Copyright (c) 2004                                                    */
/* Daniel Sleator, David Temperley, and John Lafferty                    */
/* Copyright 2008, 2009, 2013, 2014 Linas Vepstas                        */
/* All rights reserved                                                   */
/*                                                                       */
/* Use of the link grammar parsing system is subject to the terms of the */
/* license set forth in the LICENSE file included with this software.    */
/* This license allows free redistribution and use in source and binary  */
/* forms, with or without modification, subject to certain conditions.   */
/*                                                                       */
/*************************************************************************/

#include "api-structures.h"
#include "linkage.h"
#include "post-process/post-process.h" // for linkage_free_pp_info
#include "tokenize/wordgraph.h"        // for gwordlist_free
#include "linkage/lisjuncts.h"         // for lg_free_disjunct_strings
#include "utilities.h"

void free_linkage(Linkage linkage)
{
	exfree((void *) linkage->word, sizeof(const char *) * linkage->num_words);
	exfree(linkage->chosen_disjuncts, linkage->num_words * sizeof(Disjunct *));
	free(linkage->link_array);

	lg_free_disjunct_strings(linkage);

	linkage_free_pp_domains(linkage);

	gwordlist_free(linkage->wg_path);
	gwordlist_free(linkage->wg_path_display);
}

void free_linkages(Sentence sent)
{
	size_t in;
	Linkage lkgs = sent->lnkages;
	if (!lkgs) return;

	for (in=0; in<sent->num_linkages_alloced; in++)
	{
		free_linkage(&lkgs[in]);
	}

	free(lkgs);
	sent->num_linkages_alloced = 0;
	sent->num_linkages_found = 0;
	sent->num_linkages_post_processed = 0;
	sent->num_valid_linkages = 0;
	sent->lnkages = NULL;
}

/* Partial, but not full initialization of the linkage struct ... */
void partial_init_linkage(Sentence sent, Linkage lkg, unsigned int N_words)
{
	lkg->num_links = 0;
	// It is impossible for a planar graph to have more links than this.
	lkg->lasz = 2 * N_words;
	lkg->link_array = (Link *) malloc(lkg->lasz * sizeof(Link));
	memset(lkg->link_array, 0, lkg->lasz * sizeof(Link));

	lkg->num_words = N_words;
	lkg->cdsz =  N_words;
	lkg->chosen_disjuncts = (Disjunct **) exalloc(lkg->cdsz * sizeof(Disjunct *));
	memset(lkg->chosen_disjuncts, 0, N_words * sizeof(Disjunct *));

	lkg->disjunct_list_str = NULL;
	lkg->pp_domains = NULL;
	lkg->sent = sent;
}
