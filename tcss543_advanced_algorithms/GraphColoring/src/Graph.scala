
object Graph {
  def initialize(numVertices: Int,
                 edgeDensity: Double): Graph = {

    val r = scala.util.Random
    val vertices = Array.fill(numVertices)(Vertex())

    for (v <- vertices) {
      vertices.map(f => f.color).filter(c => c > 4)

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
  /**
    * Outputs edge matrix with colors (c).
    *
    *       A(c) B(c) C(c) ...
    * A(c)   0    0    0
    * B(c)   0    0    1
    * C(c)   1    0    0
    * ...
    *
    * @return
    */
  override def toString: String = {
    val stb = new StringBuilder
    val nrVertices = vertices.length
  }
}
