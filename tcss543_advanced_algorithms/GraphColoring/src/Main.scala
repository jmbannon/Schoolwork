object Main {

  def main(args: Array[String]): Unit = {
    val graph = Graph.initialize(10, 0.5)
    println(graph.toString)

    graph.color()

    println(graph.verify())
  }
}
