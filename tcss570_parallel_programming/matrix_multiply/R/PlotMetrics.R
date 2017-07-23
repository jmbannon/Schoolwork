

editframe <- function(csvpath, first) {
  library(e1071); library(ggplot2); library(reshape); library(dplyr)
  
  cbPalette <- c("#999999", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")
  frame <- read.csv(csvpath, header = FALSE, stringsAsFactors = FALSE)
  frame <- frame[,-3]
  
  for (i in 1:nrow(frame)) {
    if (frame[i,1] %in% "single_thread") {
      frame[i,1] <- "Single-Threaded"
      frame[i,4] <- 0
    } else if (frame[i,1] %in% "multi_thread") {
      frame[i,1] <- "Multi-Threaded"
      frame[i,4] <- 1
    } else {
      frame[i,1] <- sprintf("Strassen %d", frame[i,4])
    }
  }
  
  colnames(frame) <- c("Implementation", "threads", "dim", "mindim", "time")
  
  metrics_1374 <- frame[frame[,3] %in% c("1374-by-1374", "2048-by-2048"),]
  metrics_3937 <- frame[frame[,3] %in% c("3937-by-3937", "4096-by-4096"),]
  
  metrics_1374 <- metrics_1374[,c(-3, -4)]
  metrics_3937 <- metrics_3937[,c(-3, -4)]
  
  metrics_1374.m <- melt(metrics_1374, id.vars=c("Implementation", "threads"), measure.vars="time")
  metrics_3937.m <- melt(metrics_3937, id.vars=c("Implementation", "threads"), measure.vars="time")
  
  metrics_1374.m$Implementation <- factor(metrics_1374.m$Implementation, levels = c("Single-Threaded", "Multi-Threaded", "Strassen 32", "Strassen 64", "Strassen 128", "Strassen 256", "Strassen 512", "Strassen 1024"))
  metrics_3937.m$Implementation <- factor(metrics_3937.m$Implementation, levels = c("Single-Threaded", "Multi-Threaded", "Strassen 32", "Strassen 64", "Strassen 128", "Strassen 256", "Strassen 512", "Strassen 1024"))
  
  if (first) {
  ggplot(metrics_1374.m, aes(x=as.factor(threads), y=value, fill=Implementation)) + 
    geom_bar(stat="identity", position = "dodge") + 
    xlab("Threads") + 
    ylab("Runtime (sec)") +
    scale_y_continuous(breaks = scales::pretty_breaks(n = 10)) +
    ggtitle("1374-by-1374 Matrix Multiply") +
    theme(plot.title = element_text(hjust = 0.5)) +
    scale_fill_manual(values=cbPalette)
  } else {

  ggplot(metrics_3937.m, aes(x=as.factor(threads), y=value, fill=Implementation)) + 
    geom_bar(stat="identity", position = "dodge") + 
    xlab("Threads") + 
    ylab("Runtime (sec)") +
    scale_y_continuous(breaks = scales::pretty_breaks(n = 10)) +
    ggtitle("3937-by-3937 Matrix Multiply") +
    theme(plot.title = element_text(hjust = 0.5)) +
    scale_fill_manual(values=cbPalette)
  }
}
