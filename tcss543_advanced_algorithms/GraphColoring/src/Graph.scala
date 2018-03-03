import scala.collection.mutable

object Graph {

  /**
    * Initializes graph with number of vertices and edge density.
    */
  def initialize(numVertices: Int,
                 edgeDensity: Double): Graph = {

    val r = scala.util.Random

    val vertices = new Array[Vertex](numVertices)
    var name = 'A'.toInt
    for (i <- vertices.indices) {
      vertices.update(i, Vertex(totalVertices = numVertices, name = name.toChar))
      name += 1
    }

    // Non-directed graphs are symetrical, only consider the 'upper-triangular' portion
    for (i <- vertices.indices) {
      for (j <- (i + 1) until numVertices) {
        if (r.nextDouble() < edgeDensity) {
          // Edge must be updated for both vertices since it's non-directed
          vertices(i).edges.append(vertices(j))
          vertices(j).edges.append(vertices(i))
        }
      }
    }

    new Graph(vertices)
  }
}

class Graph(vertices: Array[Vertex]) {

  // Coloring algorithm with O(V lg|V| E)
  def color(): Unit = {

    val firstVertex = vertices.maxBy(_.adjacencyDegree)        // |V|
    firstVertex.color = firstVertex.minColor                   // lg |V|
    firstVertex.edges.foreach(e => {                           // |E|
      e.removeColor(firstVertex.color)                         // -- lg |V|
    })

    val tMap = new mutable.TreeMap[Int, mutable.TreeSet[Vertex]]()
    tMap ++= vertices                                            // |V| lg |V|
      .groupBy(_.saturationDegree)                             // |V|
      .map(s => {                                              // |V|
        val (saturationDegree, vertices) = s
        val saturationTree = new mutable.TreeSet[Vertex]()
        saturationTree ++= vertices                            // |V| lg |V|
        (saturationDegree, saturationTree)
      })
      .filter(_._2.nonEmpty)

    while (tMap.nonEmpty) {                                  // |V|
      val (maxSaturation, maxSaturationVertices) = tMap.last // -- lg |V|
      val toColor = maxSaturationVertices.last             // -- lg |V|

      maxSaturationVertices.remove(toColor)                // -- lg |V|
      if (maxSaturationVertices.isEmpty) {
        tMap.remove(maxSaturation)                           // -- lg |V|
      }


      toColor.color = toColor.minColor                     // -- lg |V|

      toColor.edges.foreach(e => {                         // -- |E|
        val eOldSaturation = e.saturationDegree
        e.removeColor(toColor.color)                                        // -- -- lg |V|

        // Remove edge from NestedSaturationTree if it exists
        // (i.e. not colored)
        if (tMap.contains(eOldSaturation) && tMap(eOldSaturation).remove(e)) {  // -- -- lg |V|

          // Remove old SaturationTree if it's empty
          if (tMap(eOldSaturation).isEmpty) {
            tMap.remove(eOldSaturation)                                       // -- -- lg |V|
          }

          // Create new SaturationTree if it does not exist
          if (!tMap.contains(e.saturationDegree)) {                           // -- -- lg |V|
            tMap += ((e.saturationDegree, new mutable.TreeSet[Vertex]()))     // -- -- lg |V|
          }

          // Add edge to new SaturationTree
          tMap(e.saturationDegree).add(e)                                     // -- -- lg |V|
        }
      })
    }
  }

  /**
    * @return True if the graph is correctly colored. False otherwise.
    */
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
