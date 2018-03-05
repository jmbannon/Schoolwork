package dsatur

import scala.collection.mutable
import scala.collection.mutable.ListBuffer

case class Vertex(totalVertices: Int,
                  name: Char) extends Ordered[Vertex] {

  val availableColors: mutable.TreeSet[Int] = new mutable.TreeSet[Int]()
  availableColors ++= Range(1, totalVertices + 1)

  var edges: ListBuffer[Vertex] = new ListBuffer[Vertex]
  var color: Int = 0

  def removeColor(color: Int): Unit = availableColors.remove(color)
  def minColor: Int = availableColors.min
  def saturationDegree: Int = totalVertices - availableColors.size
  def adjacencyDegree: Int = edges.length

  override def toString: String = name + "(" + color.toString + ")"
  override def compare(that: Vertex): Int = Integer.compare(this.name.toInt, that.name.toInt)
}
