

editframe <- function(csvpath, first) {
  library(e1071); library(ggplot2); library(reshape); library(dplyr)
  
  cbPalette <- c("#999999", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")
  frame <- read.csv(csvpath, header = FALSE, stringsAsFactors = FALSE)
  frame <- frame[,-3]
  
  for (i in 1:nrow(frame)) {
    if (frame[i,1] %in% "mpi_sparse") {
      frame[i,1] <- "Sparse"
    } else if (frame[i,1] %in% "mpi_dense") {
      frame[i,1] <- "Dense"
    }
  }
  
  colnames(frame) <- c("Implementation", "Threads", "dim", "Processes", "time")
  
  metrics_1374 <- frame[frame[,1] %in% c("Sparse"),]
  metrics_3937 <- frame[frame[,1] %in% c("Dense"),]
  
  metrics_1374 <- metrics_1374[,c(-1, -2)]
  metrics_3937 <- metrics_3937[,c(-1, -2)]
  
  metrics_1374.m <- melt(metrics_1374, id.vars=c("dim", "Processes"), measure.vars="time")
  metrics_3937.m <- melt(metrics_3937, id.vars=c("dim", "Processes"), measure.vars="time")
  
  metrics_1374.m$dim <- factor(metrics_1374.m$dim, levels = c("1374-by-1374", "3937-by-3937"))
  metrics_3937.m$dim <- factor(metrics_3937.m$dim, levels = c("1374-by-1374", "3937-by-3937"))
  
  if (first) {
  ggplot(metrics_1374.m, aes(x=as.factor(Processes), y=value, fill=dim)) + 
    geom_bar(stat="identity", position = "dodge") + 
    xlab("Processes") + 
    ylab("Runtime (sec)") +
    scale_y_continuous(breaks = scales::pretty_breaks(n = 10)) +
    ggtitle("Sparse Matrix Multiply") +
    theme(plot.title = element_text(hjust = 0.5)) +
    scale_fill_manual(values=cbPalette)
  } else {

  ggplot(metrics_3937.m, aes(x=as.factor(Processes), y=value, fill=dim)) + 
    geom_bar(stat="identity", position = "dodge") + 
    xlab("Processes") + 
    ylab("Runtime (sec)") +
    scale_y_continuous(breaks = scales::pretty_breaks(n = 10)) +
    ggtitle("Dense Matrix Multiply") +
    theme(plot.title = element_text(hjust = 0.5)) +
    scale_fill_manual(values=cbPalette)
  }
}