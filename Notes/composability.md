## Composability
First, I think it's important to define what my idea of "maximum composability" is:
- Enable reuse of existing code in scenarios the original author didn't foresee
- Allow reusers to disable/modify existing behaviors on existing code
- Allow reusers to add/extend behaviors on existing code

I think the first and foremost goal of this project should be to maximize composability of UI elements. This requires the following:
- Removal or discouragement of anything that might hinder composability (ie: Margin on templates)
- Existing UI structures to be composable in a predictable and efficient manner

If we take the task of "skinning" (re-styling) an existing element, we're faced with the following two options:
- The skinnable control can make itself parameterizable, therefore requiring the control author to be aware of all the ways the user might wish for it to be skinned
- The control user can modify the existing control upon instantiation, therefore requiring an understanding of its structure

Both options make tradeoffs, and neither one is perfect 100% of the time. I think the ideal solution would allow for both solutions to be employable simultaneously.

The API should be designed in such a way that it's very cumbersome to write things that would break the goal of composability. Violations of that should be obvious.
However, it doesn't have to be fool-proof. If people are going to write bad code, they're going to write bad code.
