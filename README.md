# OneSecondLab (OSL)

OneSecondLab (OSL) is a lightweight, context-agnostic benchmarking framework written in C for measuring and visualizing time-series data of different algorithms.

# Terminology
**Implementation:** A user-defined function representing a single algorithm (For example, Heron's Method for square roots). An implementation advances the context for its next iteration. 

**Iteration**: A single run of an implementation. An iteration (hopefully) reports new information (for example, one iteration of Heron's Method will find one new square root).

**Context:** A user-owned struct that encapsulates the data from a previous iteration needed to run the current iteration (for example, the previous iteration found the square root of n, so the next iteration will try to find the square root of n + 1). By treating context as a black box, OSL is context-agnostic, can benchmark extremely complex algorithms, and allows the user to decide what inputs the implementations receive.

**Progress Function:** A user-defined function that reports a scalar metric from the context representing some progress to be graphed (for example, a new square root computed). By allowing the user to define the progress function, the user decides what progress is and what gets graphed in the GIFs.

**Benchmark:** A user-defined configuration object that defines the context, implementations, progress function, and allotted time (can be more than one second!).

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
- For the sake of consistent GIF framerate, the iterations sampled are constant across all implementations. Therefore, a very slow algorithm will appear slightly slower than it actually is, compared to a faster algorithm.
- OSL does not work with Windows because of it's use of POSIX timing mechanisms. Future versions may address this.
- Time on the Y-axis.

# Future Features
- Benchmarking by memory usage in addition to time
- CPU utilization tracking
- Pure CLI configuration
- More flags and options (i.e, GIF length, static image, automated TikZ export etc.)
- Possibly debugging options
- Windows support

# Case Studies
TBD: my plan is to make a case study-style guide of using OSL for square roots or others. Maybe a blog post for each video of the series explaining how it's used. 




