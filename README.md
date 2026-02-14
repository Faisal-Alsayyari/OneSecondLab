# OneSecondLab (OSL)

OneSecondLab (OSL) is a lightweight, context-agnostic benchmarking framework written in C for measuring and visualizing time-series data of different algorithms.

# Terminology
**Implementation:** A user-defined function representing a single algorithm. Each implementation advances the context for its next iteration.

**Context:** A user-owned struct that encapsulates the data from a previous iteration needed to run the current iteration. By treating context as a black box, OSL is context-agnostic.

**Progress Function:** A user-defined function that reports a scalar metric from the context representing progress (for example, a new square root computed)

**Benchmark:** A user-defined configuration object that defines the context, implementations, progress function, allotted time (can be more than one second!), and target sample points. 

# Design Goals

### 1. Context-Agnostic
OSL does not assume anything about algorithm structure or the data an algorithm needs. By allowing the user to define the context and a progress function, OSL can automatically benchmark any iterative numerical algorithm.

### 2. Time-based Sampling
Instead of being progress-sampled (i.e, sample every 10 square roots), OSL is time-sampled (i.e, sample every 10 ms). This allows for cleaner time-series data and consistency across algorithms with wildly different performances and asymptotic behaviors. 

### 3. Minimal Overhead
OSL is designed to be as lightweight as possible. Overhead timing from the benchmark itself is minimized.

### 4. Explicit Ownership
Implementations are owned by the user. OSL does not validate correctness, interact with algorithm output, or otherwise interpret results.

# Limitations
- OSL does not validate the correctness of algorithm outputs.

# Future Features
- Benchmarking by memory usage in addition to time
- CPU utilization tracking
- Pure CLI configuration

# Case Studies
TBD: my plan is to make a case study-style guide of using OSL for square roots or others. Maybe a blog post for each video of the series explaining how it's used. 

