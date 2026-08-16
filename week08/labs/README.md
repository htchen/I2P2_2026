# Week 8 Lab — Final Template Build and Architecture Map

## Outcomes

Students can build and run the final-project template, reproduce a smoke test,
and navigate its major subsystems before proposing changes.

## Part A — Environment smoke test

1. Record the template release/revision, compiler, platform, and Allegro version.
2. Follow the published setup instructions from a clean checkout.
3. Build and launch the unchanged program.
4. Exercise start, one gameplay interaction, state transition, and normal exit.
5. Record exact commands and any platform-specific deviation.

Do not begin feature implementation until the unchanged baseline is reproducible.

## Part B — Architecture map

Locate and connect:

- program entry and game/event loop;
- state or scene selection;
- data, operation, image, sound, font, and other resource centers;
- object, shape, tower, monster, bullet, level, and UI hierarchies;
- construction, update, draw, removal, and shutdown paths.

Trace one input event and one game object through the map. Mark uncertain edges
as hypotheses to verify, not facts.

## Part C — AI-assisted orientation

Give an AI tool only the relevant directory listing and a small set of headers.
Ask for questions to investigate rather than a feature implementation. Verify
its proposed relationships with definitions and call sites; record one invented
or incomplete assumption.

## Deliverable

- reproducible build/smoke-test record;
- one-page subsystem/call-flow map;
- list of three verified extension seams and two risks;
- first final-project `AI_USAGE.md` entry.
