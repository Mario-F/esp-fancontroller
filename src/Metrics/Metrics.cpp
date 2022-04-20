#include <Arduino.h>
#include "Metrics.h"

MetricLabel MetricsManager::_globalLabels[MAX_GLOBAL_LABELS_COUNT];
int MetricsManager::_globalLabelCount = 0;
Metric MetricsManager::_metrics[MAX_METRICS_COUNT];
int MetricsManager::_metricCount = 0;

void MetricsManager::resetGlobalLabels() {
  _globalLabelCount = 0;
}

void MetricsManager::addGlobalLabel(String labelName, String labelValue) {
  _globalLabels[_globalLabelCount].name = labelName;
  _globalLabels[_globalLabelCount].value = labelValue;
  _globalLabelCount++;
};

Metric *MetricsManager::createMetric(String mName, String mType, String mHelp) {
  Metric *nm = &_metrics[_metricCount];
  _metricCount++;
  nm->name = mName;
  nm->help = mHelp;
  nm->type = mType;
  nm->labelCount = 0;
  nm->value = 0;
  return nm;
};

MetricLabel *MetricsManager::addMetricLabel(Metric *metric, String labelName, String labelValue) {
  MetricLabel *nml = &metric->labels[metric->labelCount];
  metric->labelCount++;
  nml->name = labelName;
  nml->value = labelValue;
  return nml;
};

void MetricsManager::resetMetrics() {
  _metricCount = 0;
};

String MetricsManager::getMetrics() {
  String result = "";

  // iterate over metrics
  for (int i = 0; i < _metricCount; i++) {
    Metric *m = &_metrics[i];
    result += "# HELP " + m->name + " " + m->help + "\n";
    result += "# TYPE " + m->name + " " + m->type + "\n";
    result += m->name;

    // Add labels if existing
    if (m->labelCount > 0 || _globalLabelCount > 0) {
      result += "{";
      bool first = true;
      for (int j = 0; j < _globalLabelCount; j++) {
        if (!first) {
          result += ",";
        }
        result += _globalLabels[j].name + "=\"" + _globalLabels[j].value + "\"";
        first = false;
      }
      for (int j = 0; j < m->labelCount; j++) {
        if (!first) {
          result += ",";
        }
        result += m->labels[j].name + "=\"" + m->labels[j].value + "\"";
        first = false;
      }
      result += "}";
    }

    result += " " + (String)m->value + "\n";
  }

  return result;
};
