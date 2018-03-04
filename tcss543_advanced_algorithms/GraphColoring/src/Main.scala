object Main {

  /** Toggles to compute average of runtime or number of colors. True for runtime. False for number of colors. */
  private val RUNTIME_NUMCOLOR_TOGGLE = false

  def main(args: Array[String]): Unit = {
    val nrTrials = 100

    // Prints density percentage at top
    print("    ")
    for (densityInt <- 5 to 95 by 5) {
      print(f"${densityInt.toDouble}%2.3f,")
    }
    println()

    for (nrVertices <- 100 to 1000 by 100) {
      print(s"$nrVertices")
      for (densityInt <- 5 to 95 by 5) {
        var elapsedTimeSum = 0.0
        var numColorsSum = 0L

        for (_ <- 0 until nrTrials) {
          val density = densityInt.toDouble / 100.0
          val graph = Graph.initialize(nrVertices, density)
          val t0 = System.currentTimeMillis()
          graph.color()
          val timeElapsedSeconds = (System.currentTimeMillis() - t0).toDouble / 1000.0
          elapsedTimeSum += timeElapsedSeconds
          numColorsSum += graph.colorCount()

          if (!graph.verify()) {
            println(s"ERROR: Invalid output. $nrVertices vertices; $density density")
            System.exit(1)
          }
        }

        val aveElapsedTime = elapsedTimeSum / nrTrials.toDouble
        val aveNumColors = numColorsSum.toDouble / nrTrials.toDouble

        if (RUNTIME_NUMCOLOR_TOGGLE)
          print(f",$aveElapsedTime%1.4f")
        else
          print(f",$aveNumColors%1.4f")

      }
      println()
    }
  }
}
