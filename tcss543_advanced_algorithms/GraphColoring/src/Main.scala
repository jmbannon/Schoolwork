object Main {

  def main(args: Array[String]): Unit = {
    val graph = Graph.initialize(8, 0.5)
    println(graph.toString)

    graph.color()

    println(graph.toString)
//    println("Hello world")
  }
}
