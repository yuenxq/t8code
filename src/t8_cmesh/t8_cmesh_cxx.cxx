/*
  This file is part of t8code.
  t8code is a C library to manage a collection (a forest) of multiple
  connected adaptive space-trees of general element classes in parallel.

  Copyright (C) 2015 the developers

  t8code is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  t8code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with t8code; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include <t8_cmesh.h>
#include <t8_element_cxx.hxx>
#include "t8_cmesh_types.h"

/** \file t8_cmesh_cxx.cxx
 *  This file collects all general cmesh routines that need c++ compilation.
 *  Particularly those functions that use the element interface frome \ref t8_element_cxx.hxx.
 *
 * TODO: document this file
 */

void
t8_cmesh_uniform_bounds (t8_cmesh_t cmesh, int level,
                         t8_scheme_cxx_t * ts,
                         t8_gloidx_t * first_local_tree,
                         t8_gloidx_t * child_in_tree_begin,
                         t8_gloidx_t * last_local_tree,
                         t8_gloidx_t * child_in_tree_end,
                         int8_t * first_tree_shared)
{
  int                 is_empty;

  T8_ASSERT (cmesh != NULL);
  T8_ASSERT (cmesh->committed);
  T8_ASSERT (level >= 0);
  T8_ASSERT (ts != NULL);

  *first_local_tree = 0;
  if (child_in_tree_begin != NULL) {
    *child_in_tree_begin = 0;
  }
  *last_local_tree = 0;
  if (child_in_tree_end != NULL) {
    *child_in_tree_end = 0;
  }

    t8_gloidx_t         global_num_children;
    t8_gloidx_t         first_global_child;
    t8_gloidx_t         last_global_child;
    t8_gloidx_t         children_per_tree = 0, passed_children = 0, shift=0;
    t8_gloidx_t         first_class_children_per_tree = -1;
    t8_locidx_t         trees;
#ifdef T8_ENABLE_DEBUG
    t8_gloidx_t         prev_last_tree = -1;
#endif
    int                 tree_class, i;
    t8_eclass_scheme_c *tree_scheme;

    /* Compute the number of children on level in each tree */
    global_num_children = 0;
    for (tree_class = T8_ECLASS_ZERO; tree_class < T8_ECLASS_COUNT;
         ++tree_class) {
      /* We iterate over each element class and get the number of children for this
       * tree class.
       * Currently we do not supported different numbers of children for different classes.
       * Thus, if we encounter this situation, we abort with an error.
       * Different numbers of children for different classes will be supported in the future.
       */
      if (cmesh->num_trees_per_eclass[tree_class] > 0) {
        tree_scheme = ts->eclass_schemes[tree_class];
        T8_ASSERT (tree_scheme != NULL);
        children_per_tree =
          tree_scheme->t8_element_count_leafs_from_root (level);
        if (first_class_children_per_tree >= 0
            && first_class_children_per_tree != children_per_tree) {
            t8_debugf("[D] is this necessary?\n");
         /* SC_ABORT
            ("Currently t8code does not support different leaf counts per tree.");*/
        }
        first_class_children_per_tree = children_per_tree;
        t8_debugf("[D] class: %i, children_per_tree: %i\n", tree_class, children_per_tree);
        global_num_children += cmesh->num_trees_per_eclass[tree_class] * children_per_tree;
        t8_debugf("[D] global_num_children: %i\n", global_num_children);
      }
    }
    T8_ASSERT (children_per_tree != 0);



    if (cmesh->mpirank == 0) {
      first_global_child = 0;
      if (child_in_tree_begin != NULL) {
        *child_in_tree_begin = 0;
      }
    }
    else {
      /* The first global child of processor p
       * with P total processor is (the biggest int smaller than)
       * (total_num_children * p) / P
       * We cast to long double and double first to prevent integer overflow.
       */
      first_global_child =
        ((long double) global_num_children *
         cmesh->mpirank) / (double) cmesh->mpisize;
    }
    if (cmesh->mpirank != cmesh->mpisize - 1) {
      last_global_child =
        ((long double) global_num_children *
         (cmesh->mpirank + 1)) / (double) cmesh->mpisize;
    }
    else {
      last_global_child = global_num_children;
    }

    T8_ASSERT (0 <= first_global_child
               && first_global_child <= global_num_children);
    T8_ASSERT (0 <= last_global_child
               && last_global_child <= global_num_children);
    t8_debugf("[D] first_global_child: %i\n", first_global_child);
    t8_debugf("[D] last_global_child: %i\n", last_global_child);

    /*trees = -1;
    do{
        tree_class = t8_cmesh_get_tree_class(cmesh, trees + 1);
        tree_scheme = ts->eclass_schemes[tree_class];
        shift = tree_scheme->t8_element_count_leafs_from_root(level);
        passed_children += shift;
        trees++;
        t8_debugf("[D] trees: %i localTrees: %i\n", trees,
                  t8_cmesh_get_num_local_trees(cmesh));
    } while (passed_children < first_global_child);
    passed_children-=shift;

    t8_debugf("[D] trees: %i, passed_children: %i\n", trees, passed_children);*/

    *first_local_tree = trees;
    *first_local_tree = first_global_child / children_per_tree;
    if (child_in_tree_begin != NULL) {
       *child_in_tree_begin =
        first_global_child - *first_local_tree * children_per_tree;
        //*child_in_tree_begin = first_global_child - passed_children;
    }
    /*t8_debugf("[D] last_global_child: %i\n", last_global_child);
    trees = -1;
    passed_children = 0;
    do{
        tree_class = t8_cmesh_get_tree_class(cmesh,trees+1);
        tree_scheme = ts->eclass_schemes[tree_class];
        shift = tree_scheme->t8_element_count_leafs_from_root(level);
        passed_children += shift;
        trees++;
        t8_debugf("[D] trees: %i localTrees: %i\n", trees,
                  t8_cmesh_get_num_local_trees(cmesh));
    }while(passed_children < last_global_child &&
           trees+1 < t8_cmesh_get_num_local_trees(cmesh));
    passed_children-=shift;
    t8_debugf("[D]llt: %i, passed: %i\n", trees, passed_children);*/
    //*last_local_tree = trees;
    *last_local_tree = (last_global_child - 1) / children_per_tree;

    is_empty = *first_local_tree >= *last_local_tree
      && first_global_child >= last_global_child;
    if (first_tree_shared != NULL) {
#ifdef T8_ENABLE_DEBUG
      prev_last_tree = (first_global_child - 1) / children_per_tree;
      T8_ASSERT (cmesh->mpirank > 0 || prev_last_tree <= 0);
#endif
      if (!is_empty && cmesh->mpirank > 0 && first_global_child > 0) {
        /* We exclude empty partitions here, by def their first_tree_shared flag is zero */
        /* We also exclude that the previous partition was empty at the beginning of the
         * partitions array */
        /* We also exclude the case that we have the first global element but
         * are not rank 0. */
        *first_tree_shared = 1;
      }
      else {
        *first_tree_shared = 0;
      }
    }
    if (child_in_tree_end != NULL) {
      if (*last_local_tree > 0) {
        *child_in_tree_end =
          last_global_child - *last_local_tree * children_per_tree;
          //*child_in_tree_end = last_global_child - passed_children;
      }
      else {
        *child_in_tree_end = last_global_child;
      }
      t8_debugf("[D] llt: %i, child_in_tree_end: %i\n", *last_local_tree,
                *child_in_tree_end);
    }
    if (is_empty) {
      /* This process is empty */
      /* We now set the first local tree to the first local tree on the
       * next nonempty rank, and the last local tree to first - 1 */
      *first_local_tree = last_global_child / children_per_tree;
      if (first_global_child % children_per_tree != 0) {
        /* The next nonempty process shares this tree. */
        (*first_local_tree)++;
      }

      *last_local_tree = *first_local_tree - 1;
    }

#if 0
    if (first_global_child >= last_global_child && cmesh->mpirank != 0) {
      /* This process is empty */
      *first_local_tree = prev_last_tree + 1;
    }
#endif
  /*}
  else {
    SC_ABORT ("Partition with level > 0 "
              "does not support pyramidal elements yet.");
  }*/
}
