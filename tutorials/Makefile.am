# This file is part of t8code
# Non-recursive Makefile.am in tutorials
# Included from toplevel directory

bin_PROGRAMS += \
	tutorials/general/t8_step0_helloworld \
	tutorials/general/t8_step1_coarsemesh \
	tutorials/general/t8_step2_uniform_forest \
	tutorials/general/t8_step3_adapt_forest \
	tutorials/general/t8_step4_partition_balance_ghost \
	tutorials/general/t8_step5_element_data \
	tutorials/general/t8_step5_element_data_c_interface \
	tutorials/general/t8_tutorial_search \
	tutorials/features/t8_features_curved_meshes

tutorials_general_t8_step0_helloworld_SOURCES = \
	tutorials/general/t8_step0_helloworld.c
tutorials_general_t8_step1_coarsemesh_SOURCES = \
	tutorials/general/t8_step1_coarsemesh.c
tutorials_general_t8_step2_uniform_forest_SOURCES = \
	tutorials/general/t8_step2_uniform_forest.cxx
tutorials_general_t8_step3_adapt_forest_SOURCES = \
	tutorials/general/t8_step3_adapt_forest.cxx \
	tutorials/general/t8_step3_main.cxx
tutorials_general_t8_step4_partition_balance_ghost_SOURCES = \
	tutorials/general/t8_step3_adapt_forest.cxx \
	tutorials/general/t8_step4_partition_balance_ghost.cxx \
	tutorials/general/t8_step4_main.cxx
tutorials_general_t8_step5_element_data_SOURCES = \
	tutorials/general/t8_step3_adapt_forest.cxx \
	tutorials/general/t8_step5_element_data.cxx \
	tutorials/general/t8_step5_main.cxx
tutorials_general_t8_step5_element_data_c_interface_SOURCES = \
	tutorials/general/t8_step3_adapt_forest.cxx \
	tutorials/general/t8_step5_element_data_c_interface.c \
	tutorials/general/t8_step5_main.cxx
tutorials_general_t8_tutorial_search_SOURCES = \
	tutorials/general/t8_tutorial_search.cxx \
	tutorials/general/t8_step3_adapt_forest.cxx
tutorials_features_t8_features_curved_meshes_SOURCES = \
  tutorials/features/t8_features_curved_meshes.cxx

libt8_installed_headers += \
        tutorials/general/t8_step3.h \
        tutorials/general/t8_step4.h \
        tutorials/general/t8_step5.h

