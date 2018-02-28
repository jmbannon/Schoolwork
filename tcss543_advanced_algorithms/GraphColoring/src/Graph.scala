import scala.collection.mutable

object Graph {
  def initialize(numVertices: Int,
                 edgeDensity: Double): Graph = {

    val r = scala.util.Random

    val vertices = new Array[Vertex](numVertices)
    var name = 'A'.toInt
    for (i <- vertices.indices) {
      vertices.update(i, Vertex(name = name.toChar))
      name += 1
    }

    for (v <- vertices) {
      vertices.foreach(ve => {
        if (!v.eq(ve) && r.nextDouble() < edgeDensity) {
          v.edges.append(ve)
        }
      })
    }

    new Graph(vertices)
  }
}

class Graph(vertices: Array[Vertex]) {

  def color(): Unit = {
    vertices.maxBy(_.adjacencyDegree).color = 1 // V

    val pq = new mutable.PriorityQueue[Vertex]()(Ordering.by[Vertex, Int](_.saturationDegree).reverse)
    pq ++= vertices // V

    while (pq.nonEmpty) {                  // V * 2VE
      // Reorders priority queue
      pq ++= pq.dequeueAll
        .filter(_.color == 0)                                      // V
        .map(v => {                                                // V*E
          v.saturationDegree = v.edges.map(_.color).distinct.size
          v
        })

      val toColor = pq.dequeue()
      val edgeColors = toColor.edges.map(_.color).distinct
      var color = 1
      while (edgeColors.contains(color)) {
        color += 1
      }
      toColor.color = color
    }
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
