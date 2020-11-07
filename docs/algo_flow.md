# Polyphase Fibonacci Sorting

## Tape serialization

Tape has limit of memory that it can use

If memory limit is reached while reading then pages buffered in writer are flushed.
If memory limit is reached when writer doesn't have any buffered pages exception is thrown.

### Tape writing

Initial state: page size, path to tape file

Writing works on TapeWriter and PageWriter

## Series

Series is vector of records in nondecreasing order.

#### Tape Writer

It's used as the only way to write records to hard drive.

TapeWriter contains PageWriter and uses it as access to disk memory

- Every page begins with header with number of records on this page
- TapeWriter accepts series of size not exceeding (page size - page header)  and buffers them
- On flush performs fitting series to pages and saves them as byte vectors with headers at the beginning and passes them to page writer
- At the end of lifetime performs flush

#### Page Writer

Initial state: accepts vector of bytes with size below or equal page size only

Caches number of hard drive accesses.

- On page write takes a vector of bytes and writes it to hard drive

  

### Tape Reading

Initial state: page size and tape file path

Works on TapeReader and PageReader

#### Page Reader

Caches number of hard drive accesses. 

- On Read(), reads page from hard drive and returns vector of bytes.

#### Tape Reader

Translates hard drive pages into series.

Takes vector of bytes from PageReader, reads number of records from the page header, creates series of this length and buffers them.

- On next series prompt, pops series from buffer
- When buffer is empty prompts PageReader about next page.
- If there are no more pages in the tape file, then informs about end of tape

## Records distribution

- Prepare one input tape and N output tapes.
- Until input tape is not empty
  - Take next tape T
  - Take next fibonacci number F
  - Take previous fibonacci number P
  - Write F - P series to tape T
  - If input is empty, cache dummy series on tape T


## Tapes merging

Initial state: Set of tapes with at least one empty tape and two tapes with series, which numbers match Fibonacci numbers

- Choose one empty tape as output O
- Choose two tapes with a largest number of series as input I1 and I2, where I2 is lower number of series
- If I1 has D dummy series then write D series from I2 to O
- Until only one tape has records
    - Merge series from I1 and I2 until I2 is empty and write to O
    - Choose empty tape as output O and two tapes I1 and I2 again