#include "DataFactory.h"

#include "BluesteinDftData.h"
#include "BluesteinDftDataList.h"
#include "LargeTwiddleMatrix.h"
#include "LargeTwiddleMatrixList.h"
#include "TwiddleMatrix.h"
#include "TwiddleMatrixList.h"
#include "TwiddleVector.h"
#include "TwiddleVectorList.h"

struct NS(DataFactory) {
  struct NS(TwiddleVectorList) *twiddle_vector;
  struct NS(TwiddleMatrixList) *twiddle_matrix;
  struct NS(LargeTwiddleMatrixList) *large_twiddle_matrix;
  struct NS(BluesteinDftDataList) *bluestein_dft_data;
};

struct NS(DataFactory) *NS(DataFactory_new)() {
  struct NS(DataFactory) *out = NEW(struct NS(DataFactory));
  out->twiddle_vector = NS(TwiddleVectorList_new)();
  out->twiddle_matrix = NS(TwiddleMatrixList_new)();
  out->large_twiddle_matrix = NS(LargeTwiddleMatrixList_new)();
  out->bluestein_dft_data = NS(BluesteinDftDataList_new)();
  return out;
}

void NS(DataFactory_delete)(struct NS(DataFactory) *p) {
  if (p) {
    if (p->twiddle_vector) {
      NS(TwiddleVectorList_delete)(p->twiddle_vector);
      p->twiddle_vector = NULL;
    }
    if (p->twiddle_matrix) {
      NS(TwiddleMatrixList_delete)(p->twiddle_matrix);
      p->twiddle_matrix = NULL;
    }
    if (p->large_twiddle_matrix) {
      NS(LargeTwiddleMatrixList_delete)(p->large_twiddle_matrix);
      p->large_twiddle_matrix = NULL;
    }
    if (p->bluestein_dft_data) {
      NS(BluesteinDftDataList_delete)(p->bluestein_dft_data);
      p->bluestein_dft_data = NULL;
    }
    DELETE(p);
  }
}

const Complex *NS(DataFactory_twiddleVector)(struct NS(DataFactory) *p,
                                             Long size, Long twiddle_size,
                                             Long first_index) {
  struct NS(TwiddleVectorList) *c = p->twiddle_vector;
  struct NS(TwiddleVectorKey) key = (struct NS(TwiddleVectorKey)){
      .size = size, .twiddle_size = twiddle_size, .first_index = first_index};
  Complex *out = NS(TwiddleVectorList_get)(c, key);
  if (out) {
    return out;
  }
  out = NS(TwiddleVector_new)(key);
  NS(TwiddleVectorList_append)(c, key, out);
  return out;
}

const Complex *NS(DataFactory_twiddleMatrix)(struct NS(DataFactory) *p,
                                             Long size_0, Long size_1,
                                             Long twiddle_size,
                                             Long first_index_0,
                                             Long first_index_1) {
  struct NS(TwiddleMatrixList) *c = p->twiddle_matrix;
  struct NS(TwiddleMatrixKey) key =
      (struct NS(TwiddleMatrixKey)){.size_0 = size_0,
                                    .size_1 = size_1,
                                    .twiddle_size = twiddle_size,
                                    .first_index_0 = first_index_0,
                                    .first_index_1 = first_index_1};
  Complex *out = NS(TwiddleMatrixList_get)(c, key);
  if (out) {
    return out;
  }
  out = NS(TwiddleMatrix_new)(key);
  NS(TwiddleMatrixList_append)(c, key, out);
  return out;
}

const Complex *NS(DataFactory_largeTwiddleMatrix)(struct NS(DataFactory) *p,
                                                  Long size, Long twiddle_size,
                                                  int block_size) {
  struct NS(LargeTwiddleMatrixList) *c = p->large_twiddle_matrix;
  struct NS(LargeTwiddleMatrixKey) key = (struct NS(LargeTwiddleMatrixKey)){
      .size = size, .twiddle_size = twiddle_size, .block_size = block_size};
  Complex *out = NS(LargeTwiddleMatrixList_get)(c, key);
  if (out) {
    return out;
  }
  out = NS(LargeTwiddleMatrix_new)(key);
  NS(LargeTwiddleMatrixList_append)(c, key, out);
  return out;
}

const struct NS(BluesteinDftData) *NS(DataFactory_bluesteinDftData)(
    struct NS(DataFactory) *p, Long dft_size, Long convolution_size) {
  struct NS(BluesteinDftDataList) *c = p->bluestein_dft_data;
  struct NS(BluesteinDftDataKey) key = (struct NS(BluesteinDftDataKey)){
      .dft_size = dft_size, .convolution_size = convolution_size};
  struct NS(BluesteinDftData) *out = NS(BluesteinDftDataList_get)(c, key);
  if (out) {
    return out;
  }
  out = NS(BluesteinDftData_new)(p, key);
  NS(BluesteinDftDataList_append)(c, key, out);
  return out;
}
