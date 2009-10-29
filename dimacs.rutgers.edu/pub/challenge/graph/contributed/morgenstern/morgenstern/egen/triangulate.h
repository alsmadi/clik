/* FILE : triangulate1.h
 * written by craig morgenstern
 *
 * Construct a random triangulation of a circuit.  Care is taken not to
 * add edges already in existence.  It is assumed that the graph containing
 * the circuit can be embedded in the plane so that (1) the region bounded by
 * the circuit is empty and (2) no edges cross.  Note that these requirements
 * are stronger than just requiring planarity.  If (2) cannot be satisfied
 * without violating (1) ( as can happen if we only required planarity ), then
 * the circuit can still be triangulated by breaking it up into the necessary
 * subcircuits and calling this routine on each subcircuit.
 */

void Triangulate1(/* int N;  unsigned *seed */);
void Triangulate2(/* int N;  unsigned *seed */);
