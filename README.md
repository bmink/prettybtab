# prettybtab

Usage: prettybtab [notes]

       notes - e.g. "E0:A2::D3:::G3:D3"

The above example argument produces:

G --------------------3-------------------------------------------------------
D -----------3-----------3----------------------------------------------------
A -----2----------------------------------------------------------------------
E --0-------------------------------------------------------------------------
    E  B     F        A# F  

Note, prettybtab is basic and does not have a lot of features. The idea is
that you call it frequently, feeding it enough notes for a line or so, then
copy+paste into your tab file. It gives you a basic outline which you can then
further tweak and by adding renotations for hammer-ons, slides and such.

When called without an argument, prettybtab prints an empty bass tab line.
