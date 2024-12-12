# Examples.

## Complex DFT.

### One dimensional DFT.

A one dimensional in-place DFT of size 128.
```C
int dft_size = 128;
jomfftComplexDftHandle handle = 0;
jomfftComplexDft(&handle, 1, &dft_size, jomfftInplace);
```

### Two dimensional DFT.

A two dimensional out-of-place DFT of size 64x64.
```C
int dft_sizes[] = {64, 64};
jomfftComplexDftHandle handle = 0;
jomfftComplexDft(&handle, 2, dft_sizes, jomfftNotInplace);
```

A two dimensional in-place DFT of size 30x30, embedded in workspace of 30x32.
```C
int dft_sizes[] = {30, 30};
int storage_sizes[] = {30, 32};
jomfftComplexDftHandle handle = 0;
jomfftComplexDftBatch(&handle, 2, dft_sizes, 1, 
                      storage_sizes, 1, 0,
                      storage_sizes, 1, 0, jomfftInplace);
```

A two dimensional out-of-place DFT of size 30x30, with output embedded in workspace of 30x32.
```C
int dft_sizes[] = {30, 30};
int out_storage_sizes[] = {30, 32};
jomfftComplexDftHandle handle = 0;
jomfftComplexDftBatch(&handle, 2, dft_sizes, 1, 
                      dft_sizes, 1, 0,
                      out_storage_sizes, 1, 0, jomfftNotInplace);
```

### Two dimensional DFT with batch processing.

A batch of two dimensional in-place DFT of size 256x256.
```C
int batch_size = 4;
int dft_sizes[] = {256, 256};
int stride = 1;
int distance = dft_sizes[0] * dft_sizes[1];
jomfftComplexDftHandle handle = 0;
jomfftComplexDftBatch(&handle, 2, dft_sizes, batch_size,
                      dft_sizes, stride, distance,
                      dft_sizes, stride, distance, jomfftInplace);
```

A batch of two dimensional in-place DFT of size 256x256, with consecutive layout at the batch axis.
```C
int batch_size = 4;
int dft_sizes[] = {256, 256};
int stride = batch_size;
int distance = 1;
jomfftComplexDftHandle handle = 0;
jomfftComplexDftBatch(&handle, 2, dft_sizes, batch_size,
                      dft_sizes, stride, distance,
                      dft_sizes, stride, distance, jomfftInplace);
```

## Real DFT.

### One dimensional DFT.

A one dimensional in-place Real DFT of size 128. In this case, input data is a real vector of size 128 with two extra workspace at its tail and output data is a complex vector of size 65 ($=128 / 2 + 1$).
```C
int dft_size = 128;
jomfftRealDftHandle handle = 0;
jomfftRealDft(&handle, 1, &dft_size, jomfftInplace);
```

A one dimensional out-of-place Real DFT of size 128. In this case, input data is a real vector of size 128 and output data is a complex vector of size 65 ($=128 / 2 + 1$).
```C
int dft_size = 128;
jomfftRealDftHandle handle = 0;
jomfftRealDft(&handle, 1, &dft_size, jomfftNotInplace);
```

### Two dimensional DFT.

A two dimensional in-place DFT of size 64x64. In this case, input data is a real matrix of size 128x128 embedded in a workspace of 128x130 and output data is a complex matrix of size 128x65 (=$128 / 2 + 1).
```C
int dft_sizes[] = {64, 64};
jomfftRealDftHandle handle = 0;
jomfftRealDft(&handle, 2, dft_sizes, jomfftInplace);
```

A two dimensional out-of-place DFT of size 64x64. In this case, input data is a real matrix of size 128x128 and output data is a complex matrix of size 128x65 (=$128 / 2 + 1).
```C
int dft_sizes[] = {64, 64};
jomfftRealDftHandle handle = 0;
jomfftRealDft(&handle, 2, dft_sizes, jomfftNotInplace);
```

A two dimensional in-place DFT of size 60x60, embedded in a real workspace of size 60x64.
```C
int dft_sizes[] = {60, 60};
int in_storage_sizes[] = {60, 64};
int out_storage_sizes[] = {60, 32};
jomfftRealDftHandle handle = 0;
jomfftRealDftBatch(&handle, 2, dft_sizes, 
                   in_storage_sizes, 1, 0,
                   out_storage_sizes, 1, 0, jomfftInplace);
```

A two dimensional out-of-place DFT of size 60x60, with output embedded in a complex workspace of size 60x32.
```C
int dft_sizes[] = {60, 60};
int out_storage_sizes[] = {60, 32};
jomfftRealDftHandle handle = 0;
jomfftRealDftBatch(&handle, 2, dft_sizes, 
                   dft_sizes, 1, 0,
                   out_storage_sizes, 1, 0, jomfftNotInplace);
```

### Two dimensional DFT with batch processing.

A batch of two dimensional in-place DFT of size 256x256. Input data is a real matrix of size 256x256 embedded in a workspace of size 256x258 and output data is a complex matrix of size 256x129 (=$256 / 2 + 1).
```C
int batch_size = 4;
int dft_sizes[] = {256, 256};
int in_storage_sizes[] = {256, 258};
int out_storage_sizes[] = {256, 129};
int stride = 1;
int in_distance = in_storage_sizes[0] * in_storage_sizes[1];
int out_distance = out_storage_sizes[0] * out_storage_sizes[1];
jomfftRealDftHandle handle = 0;
jomfftRealDftBatch(&handle, 2, dft_sizes, batch_size,
                  in_storage_sizes, stride, in_distance,
                  out_storage_sizes, stride, out_distance, jomfftInplace);
```

A batch of two dimensional out-of-place DFT of size 256x256, with consecutive layout at the batch axis. Output data is a complex matrix of size 256x129 (=$256 / 2 + 1).
```C
int batch_size = 4;
int dft_sizes[] = {256, 256};
int out_storage_sizes[] = {256, 129};
int stride = batch_size;
int distance = 1;
jomfftRealDftHandle handle = 0;
jomfftRealDftBatch(&handle, 2, dft_sizes, batch_size,
                   dft_sizes, stride, distance,
                   out_storage_sizes, stride, distance, jomfftNotInplace);
```
