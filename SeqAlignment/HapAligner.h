#ifndef HAP_ALIGNER_H_
#define HAP_ALIGNER_H_

#include <string>
#include <vector>

#include "AlignmentData.h"
#include "../base_quality.h"
#include "Haplotype.h"

class HapAligner {
 private:
  Haplotype* haplotype_;

  /**
   * Align the sequence contained in SEQ_0 -> SEQ_N using the recursion
   * 0 -> 1 -> 2 ... N
   **/
  void align_left_flank(const char* seq_0, int seq_len,
			const double* base_log_wrong, const double* base_log_correct,
			double* match_matrix, double* insert_matrix, double* deletion_matrix, double& left_prob);

  /**                                                                                                                                                                         
   * Align the sequence contained in SEQ_N -> SEQ_END using the recursion
   * END -> END-1 -> END-2 ... N
   **/
  void align_right_flank(const char* seq_n, int seq_len,
			 const double* base_log_wrong, const double* base_log_correct,
                         double* match_matrix, double* insert_matrix, double* deletion_matrix, double& right_prob);

  /**
   * Compute the log-probability of the alignment given the 
   * alignment matrices for the left and right segments
   **/
  double compute_aln_logprob(int base_seq_len, int seed_base,
			     char seed_char, double log_seed_wrong, double log_seed_correct,
			     double* l_match_matrix, double* l_insert_matrix, double* l_deletion_matrix, double l_prob,
			     double* r_match_matrix, double* r_insert_matrix, double* r_deletion_matrix, double r_prob);

 public:
  HapAligner(Haplotype* haplotype){
    haplotype_ = haplotype;
  }

  /** 
   * Returns the 0-based index into the sequence string that should be 
   * used as the seed for alignment. Returns -1 iff no valid seed exists 
   **/
  int calc_seed_base(Alignment& alignment);


  void process_reads(std::vector<Alignment>& alignments, int init_read_index, BaseQuality* base_quality,
		     double* aln_probs, int* seed_positions);


  /*
    Retraces the Alignment's optimal alignment to the provided haplotype.
    Returns the result as a new Alignment relative to the reference haplotype
   */
  Alignment trace_optimal_aln(Alignment& alignment, int seed_base, int best_haplotype);
};

#endif
