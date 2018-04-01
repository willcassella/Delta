## XML as a storage format
Goals for UI description language:
	- Easy to analyze (for optimization)
	- Easy to generate (for tools)
	- Easy to read with regards to functional paradigm
	- Easy to write with regards to functional paradigm

	So really I want something that fits well with the "functional data-description" idea, that's easily consumed/produced by both humans and tools

XML is nice, since it's what most UI designers are comfortable working with, but it has a few pitfalls with regards to what I'm trying to acheive.

I think in some cases I might be better off with an actual scripting language to acheive what I'm trying to do, but that might violate goal #1

Problems with XML:
	- Structure doesn't give enough options with regards to what means what
		- Has attributes and elements, but I think ideally I would be able to specify different types of elements
			- Don't what to force UI builders to basically write a syntax tree in XML

Basic ideas behind FDD:
- Templates are pure functions, that consume state and UI elements, and produce state and UI elements

Functional programming has some neat features like currying, pattern matching, and composition/fusion
Composition/fusion should absolutely be a priority for this, not sure about currying/pattern matching though
	- Could be solved by better use of composition

So at what point would I be better off with a programming language?
Of course it would be *possible* to do everything with XML, but I think after a certain point it's defeating goals #1, #3, and #4

Rather than pattern matching, what I could do is some really wimpy version of decomposing arguments

So what does rule #1 mean?
- Control flow must be easy to handle
	- Current ideas are just enable/disable node, or repeat node
		- Easy to understand what's going on and how to treat the cache

Rule #2 just means that it has a well-defined structure, which could be acheived by either XML or something Lispy, or something else

What is the point of the cache?
- To minimize work necessary between frames
	- If you have to revisit the model, regenerate the layout, recompute layout, and regenerate geometry every single frame that's going to be too much work
	
