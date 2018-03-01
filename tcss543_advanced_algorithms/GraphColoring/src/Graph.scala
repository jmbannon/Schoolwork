import scala.collection.mutable

object Graph {
  def initialize(numVertices: Int,
                 edgeDensity: Double): Graph = {

    val r = scala.util.Random

    val vertices = new Array[Vertex](numVertices)
    var name = 'A'.toInt
    for (i <- vertices.indices) {
      vertices.update(i, Vertex(totalVertices = numVertices, name = name.toChar))
      name += 1
    }

    for (i <- vertices.indices) {
      for (j <- (i + 1) until numVertices) {
        if (r.nextDouble() < edgeDensity) {
          vertices(i).edges.append(vertices(j))
          vertices(j).edges.append(vertices(i))
        }
      }
    }

    new Graph(vertices)
  }
}

class Graph(vertices: Array[Vertex]) {

  // Coloring algorithm with O(V lg^2|V| E)
  def color(): Unit = {

    val firstVertex = vertices.maxBy(_.adjacencyDegree)        // |V|
    firstVertex.color = firstVertex.minColor                   // lg |V|
    firstVertex.edges.foreach(e => {                           // |E|
      e.removeColor(firstVertex.color)                         // -- lg |V|
    })

    val pq = new mutable.TreeMap[Int, mutable.TreeSet[Vertex]]()

    val pqSat0 = new mutable.TreeSet[Vertex]()
    pqSat0 ++= vertices.filter(v => v.color == 0 && v.saturationDegree == 0)

    val pqSat1 = new mutable.TreeSet[Vertex]()
    pqSat1 ++= vertices.filter(v => v.color == 0 && v.saturationDegree == 1)

    if (pqSat0.nonEmpty) {
      pq += ((0, pqSat0))
    }
    if (pqSat1.nonEmpty) {
      pq += ((1, pqSat1))
    }

    while (pq.nonEmpty) {                                  // |V|
      val (maxSaturation, maxSaturationVertices) = pq.last // -- lg |V|
      val toColor = maxSaturationVertices.last             // -- lg |V|

      maxSaturationVertices.remove(toColor)                // -- lg |V|
      if (maxSaturationVertices.isEmpty) {
        pq.remove(maxSaturation)                           // -- lg |V|
      }


      toColor.color = toColor.minColor                     // -- lg |V|

      toColor.edges.foreach(e => {                         // -- |E|
        val eOldSaturation = e.saturationDegree
        e.removeColor(toColor.color)                                        // -- -- lg |V|

        // Remove edge from NestedSaturationTree if it exists
        // (i.e. not colored)
        if (pq.contains(eOldSaturation) && pq(eOldSaturation).remove(e)) {  // -- -- lg |V|

          // Remove old SaturationTree if it's empty
          if (pq(eOldSaturation).isEmpty) {
            pq.remove(eOldSaturation)                                       // -- -- lg |V|
          }

          // Create new SaturationTree if it does not exist
          if (!pq.contains(e.saturationDegree)) {                            // -- -- lg |V|
            pq += ((e.saturationDegree, new mutable.TreeSet[Vertex]()))      // -- -- lg |V|
          }

          // Add edge to new SaturationTree
          pq(e.saturationDegree).add(e)                                      // -- -- lg |V|
        }
      })
    }
  }

  def verify(): Boolean = {
    !vertices
      .map(v => (v.color, v.edges.map(_.color)))
      .exists(tuple => tuple._1 == 0 || tuple._2.contains(tuple._1))
  }


  /**
    * Outputs edge matrix with colors (c).
    *
    *       A(c) B(c) C(c) ...
    * A(c)    0    0    0
    * B(c)    0    0    1
    * C(c)    1    0    0
    * ...
    *
    * @return
    */
  override def toString: String = {
    val stb = new StringBuilder

    // Header row
    stb.append("     ")
    for (v <- vertices) {
      stb.append(v.toString)
      stb.append(" ")
    }
    stb.append('\n')

    // Each Vertex's edges
    for (v <- vertices) {
      stb.append(v.toString)
      stb.append("   ")
      for (ve <- vertices) {
        stb.append(if (v.edges.exists(_.eq(ve))) "1" else "0")
        stb.append("    ")
      }
      stb.append('\n')
    }

    stb.toString()
  }
}
