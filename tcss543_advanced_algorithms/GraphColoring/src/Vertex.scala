import scala.collection.mutable.ListBuffer

case class Vertex(edges: ListBuffer[Vertex] = new ListBuffer[Vertex],
                  var color: Int = 0,
                  var saturationDegree: Int = 0,
                  var name: Char = '!') {

  def adjacencyDegree: Int = edges.length
  override def toString: String = name + "(" + color.toString + ")"


}
