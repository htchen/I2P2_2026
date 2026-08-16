# Week 9 Lab — Event Loop and Resource Lifecycle

## Outcomes

Students can distinguish input, update, and draw responsibilities and explain
how one external resource is acquired and released on normal and failure paths.

## Part A — AI-free readiness

Given a short C++ program with nested RAII objects and one exception, write the
construction/destruction order before running it. Correct the trace using output.

## Part B — Runtime trace

Select one observable action and record:

```text
Allegro event → dispatch/state → stored intent → update → model change → draw
```

Name the functions and objects actually encountered. Identify any global or
center-mediated dependency and whether it owns or merely exposes data.

## Part C — Resource trace

Choose one display, timer, queue, image, font, sound, or GIF resource. Record its
acquisition, owner, borrowers, failure signal, normal release, and partial-init
cleanup. Propose one bounded RAII improvement, but implement it only if approved
for the milestone.

## Part D — AI review

Ask AI to review the two traces for missing failure/lifetime edges. Verify every
claim in the source. Do not accept a broad framework rewrite.

## Deliverable

- event/update/draw trace;
- resource-lifecycle table;
- one verified risk and one bounded improvement proposal;
- AI-use record and baseline smoke-test result.
