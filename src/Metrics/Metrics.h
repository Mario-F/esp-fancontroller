// Handle genration and output of metrics

#include <Arduino.h>

#define MAX_GLOBAL_LABELS_COUNT 4
#define MAX_LOCAL_LABELS_COUNT 8
#define MAX_METRICS_COUNT 32

struct MetricLabel {
  String name;
  String value;
};

struct Metric {
  String name;
  String help;
  String type;
  MetricLabel labels[MAX_LOCAL_LABELS_COUNT];
  int labelCount;
  float value;
};

class MetricsManager {

  public:
    static void resetGlobalLabels();
    static void addGlobalLabel(String labelName, String labelValue);
    static void resetMetrics();
    static Metric *createMetric(String mName, String mType, String mHelp);
    static MetricLabel *addMetricLabel(Metric *metric, String labelName, String labelValue);
    static String getMetrics();

  private:
    MetricsManager();
    static MetricLabel _globalLabels[MAX_GLOBAL_LABELS_COUNT];
    static int _globalLabelCount;
    static Metric _metrics[MAX_METRICS_COUNT];
    static int _metricCount;
};
