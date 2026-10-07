# DioramaOS Constitution

## Purpose

DioramaOS exists to bring narrative models and dioramas to life through **Lights, Sounds, and Animations**.

It provides reusable hardware and software patterns for adding electronic effects to physical models without requiring every new diorama to become a new electronics experiment.

DioramaOS is not an end unto itself.

**The story comes first.**

Technology exists to support the model, the scene, and the story being told. A feature that is technically impressive but adds nothing meaningful to the experience should be questioned.

The best DioramaOS effect may be the one nobody notices as technology at all.

---

# Core Effect Domains

DioramaOS recognizes three fundamental categories of effects:

## Lights

Anything whose primary purpose is visual illumination.

Examples include:

- steady illumination
- blinking
- fading
- flickering
- pulsing
- sequenced lighting
- synchronized lighting

## Sounds

Anything whose primary purpose is audible playback.

Examples include:

- dialogue
- music
- environmental audio
- machinery
- sound effects
- randomized audio
- sequenced audio

## Animations

Anything whose primary purpose is physical movement.

Examples include:

- servo movement
- DC motors
- stepper motors
- fans
- rotating assemblies
- linear motion
- mechanically sequenced effects

These domains describe **what the diorama does**, not the hardware used to accomplish it.

An LED is not a DioramaOS domain.

A DFPlayer is not a DioramaOS domain.

A servo controller is not a DioramaOS domain.

They are implementations of Lights, Sounds, and Animations.

---

# Triggers Cause Effects

Effects happen because something causes them to happen.

Triggers may include:

- buttons
- switches
- timers
- sensors
- random events
- completion of another effect
- startup
- external signals

Triggers are not themselves effect domains.

A trigger may initiate one effect or coordinate several Lights, Sounds, and Animations into a larger sequence.

DioramaOS should make those relationships understandable.

---

# The Diorama Comes First

DioramaOS exists to serve the physical build.

Electronics should enhance the narrative rather than become an excuse to demonstrate electronics.

Movement should exist because movement improves the scene.

Sound should exist because sound improves the scene.

Lighting should exist because lighting improves the scene.

**Just because DioramaOS can do something does not mean the diorama should.**

Restraint is a feature.

---

# Non-Blocking by Design

DioramaOS must remain responsive while effects are running.

Timing should use elapsed-time mechanisms such as `millis()` rather than blocking mechanisms such as `delay()`.

Lights, Sounds, Animations, and trigger handling must be capable of progressing concurrently without one unnecessarily preventing another from operating.

An effect waiting for something does not mean DioramaOS should wait with it.

Prefer actual state information when hardware provides it. If an audio player provides a BUSY signal, observe the BUSY signal rather than guessing how long an audio file takes to play.

Blocking behavior is an architectural exception and must be explicitly justified.

A short blocking delay required during hardware initialization may occasionally be reasonable.

Building application behavior around blocking delays is not.

---

# Prefer State Over Assumptions

Whenever practical, DioramaOS should determine what the system **is actually doing** rather than infer it from what the software expected to happen.

Prefer:

- completion signals over estimated durations
- sensor state over assumed position
- explicit state machines over chains of delays
- measured behavior over undocumented assumptions

The physical world has an annoying habit of disagreeing with software.

Believe the physical world.

---

# Simple Before Clever

DioramaOS should use the simplest implementation that clearly solves the problem.

Avoid unnecessary:

- abstraction
- frameworks
- dependencies
- hardware
- indirection
- configuration
- cleverness

Reusable does not mean abstracting everything.

Extensible does not mean predicting every future requirement.

Build what today's diorama needs in a way that leaves tomorrow's diorama somewhere reasonable to go.

---

# Hardware Is Replaceable

DioramaOS must not define itself by particular components.

The current implementation may use an Arduino Nano.

Another implementation may not.

The current audio system may use a DFPlayer Mini and PAM8302A.

Another implementation may not.

The current power system may use an LM2596.

Another implementation may not.

Hardware choices are implementations and should be documented as such.

The concepts of Lights, Sounds, Animations, triggers, sequences, and state should survive changes in hardware.

---

# Hardware and Software Are One System

A DioramaOS release is not merely source code.

The behavior of the system depends upon software **and physical implementation**.

Therefore the project should document, where applicable:

- firmware
- wiring
- pin assignments
- power requirements
- component specifications
- connectors
- configuration
- audio assets
- mechanical interfaces
- hardware revisions

A future maintainer should not have to reverse-engineer a finished diorama merely to understand how it was built.

---

# Build for the Workbench

DioramaOS hardware should be designed with construction, testing, repair, and modification in mind.

Prefer:

- modular assemblies
- removable electronics
- connectors over permanently tethered wiring
- clearly identified connections
- testable subsystems
- replaceable components
- accessible service points

The fact that electronics will eventually disappear underneath a model is an argument for **better documentation and serviceability**, not less.

---

# Known-Good Matters

Once a hardware/software combination has been demonstrated to work, preserve enough information to reproduce it.

Photographs are documentation.

Wiring diagrams are documentation.

Measurements are documentation.

Test procedures are documentation.

A known-good prototype should be captured before it is dismantled.

When troubleshooting, change one meaningful variable at a time whenever practical.

Measure before guessing.

---

# Comments Explain Why

DioramaOS favors generous, useful comments.

Comments should document:

- intent
- constraints
- hardware behavior
- timing assumptions
- surprising behavior
- workarounds
- design reasoning
- things future maintainers might reasonably misunderstand

Comments should not merely translate code into English.

```cpp
digitalWrite(LED_PIN, HIGH); // Turn LED on
```

does not teach anyone anything.

A comment explaining **why this LED must remain on while another asynchronous operation occurs** might.

There is no prize for the fewest comments.

---

# Code May Have a Personality

Readable code does not have to be sterile.

DioramaOS documentation and comments may be conversational, humorous, and human when doing so does not obscure technical meaning.

A memorable explanation is often a useful explanation.

Humor must never substitute for accuracy.

Clever jokes must never require clever code.

If future maintainers learn both **why the DFPlayer BUSY pin goes LOW** and **why R2 must wait for C-3PO to finally stop talking**, the comment has done its job.

---

# Projects Are Compositions, Not Forks

Individual dioramas should consume and configure DioramaOS rather than become independent divergent copies of it whenever practical.

An R2-D2/C-3PO scene may use:

- Sounds
- Lights
- a button trigger

A robot may use:

- Lights
- Sounds
- Animations
- timed sequences

A spacecraft scene may use:

- Lights
- Sounds
- motor-driven Animations

These projects should exercise and expand the platform rather than fragment it.

When a new project requires a new capability, ask whether that capability belongs in DioramaOS itself.

---

# Expansion Must Be Earned

DioramaOS should grow because actual projects reveal reusable needs.

Do not add motor-controller abstractions merely because a motor controller might someday be useful.

When a real project needs motor control, implement it thoughtfully and determine what portion is reusable.

The roadmap should be pulled primarily by real builds rather than hypothetical architecture.

---

# Decisions Should Be Recorded

Important architectural and technical decisions should be preserved along with the reasoning behind them.

Documentation should answer both:

**What did we decide?**

and:

**Why did we decide it?**

A decision that later changes was not necessarily a bad decision.

When circumstances change, supersede the old decision rather than rewriting history.

Future maintainers should be able to follow the evolution of the project.

---

# Reproducibility Matters

A DioramaOS release should represent a system that can reasonably be reconstructed.

Source control should preserve the relationship among:

- code
- configuration
- documentation
- hardware design
- project examples

Version numbers should describe known states of the platform rather than arbitrary milestones.

DioramaOS adopts Semantic Versioning beginning with **0.0.5**.

Versions before 0.0.5 represent the project's scattered experimental history and are not formal releases.

---

# Keep the Source of Truth Boring

Important project state should live in simple, obvious, version-controlled artifacts whenever practical.

Configuration should be readable.

Versions should be discoverable.

Pin assignments should be documented.

Hardware decisions should be recorded.

Someone should not need specialized tooling merely to discover what version of DioramaOS they are looking at or how it is supposed to work.

Boring files age remarkably well.

---

# Automation Serves the Project

Build pipelines, tests, versioning, and release automation exist to make DioramaOS more reliable and reproducible.

Automation should reduce repetitive work and prevent known classes of mistakes.

Automation should not exist merely because automation is possible.

A local Jenkins pipeline is useful if it can answer questions such as:

- Does the firmware compile?
- Is the version valid?
- Are expected project artifacts present?
- Can known configurations still build?
- Is this release reproducible?

The pipeline serves DioramaOS.

DioramaOS does not serve the pipeline.

---

# AI Is a Collaborator

AI may assist with:

- architecture
- implementation
- debugging
- documentation
- testing
- research
- code review
- hardware reasoning

AI recommendations are subject to the same engineering scrutiny as recommendations from any other source.

When hardware disagrees with AI, **believe the multimeter**.

Architectural decisions remain human decisions.

AI should help explain tradeoffs and preserve reasoning rather than obscure it.

---

# Learn, Do, Teach

DioramaOS is both a tool and a learning project.

Useful discoveries should be captured so they do not have to be rediscovered during the next build.

The cycle is:

**Learn.  
Do.  
Teach.**

What is learned on one diorama should make the next diorama easier to build.

Mistakes are useful when their lessons survive them.

---

# Future Jay Is a Contributor

Design and document DioramaOS for the person who opens the project years from now and says:

**“Okay...what the hell was I doing here?”**

That person may have forgotten:

- which pin was BUSY
- why a particular resistor exists
- which voltage a module requires
- why one library was chosen
- which connector goes to R2
- why `delay()` is conspicuously absent
- why there is a photograph of a breadboard in the documentation

Do that person a favor.

Assume memory leaks.

Document accordingly.

---

# Success Criteria

DioramaOS succeeds when:

- a new diorama can reuse capabilities developed for earlier builds
- Lights, Sounds, and Animations can coexist without unnecessary interference
- new hardware can be introduced without redefining the platform
- a finished installation can be understood and serviced later
- a known release can be reproduced from its documentation
- a future contributor can understand not only **how** something works, but **why it works that way**
- adding electronics makes the storytelling better rather than merely making the electronics more complicated

---

# Final Principle

DioramaOS exists because models can do more than sit there.

They can glow.

They can speak.

They can move.

They can surprise someone.

They can help tell a story.

**Build the technology necessary to make that story happen, then get the technology out of the story's way.**

