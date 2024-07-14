# Exercise in Implementing a Memory Allocator

Going to try and write it on a plane, no wifi or anything. Going off the example in the C Programming Language textbook.

I'm bored and tired while writing this, so like, don't use it for anything, purely for learning. Don't want to accidentally end up with the entirety of the Roblox tech stack relying on my code. :3

## El Plan (or L Plan, who knows)

- security? What's that?
- linked list to keep track of free list
  - going to use first fit to find room
  - defragmentation? if i have time
- Gotta force alignment by using a long
