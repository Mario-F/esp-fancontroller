#include <Arduino.h>
#include "Metrics.h"

MetricGroup MetricsManager::_metricGroups[MAX_METRICS_GROUPS_COUNT];
int MetricsManager::_metricGroupCount = 0;
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
  nm->labelCount = 0;
  nm->value = 0;

  // search metric group for this metric and add if not found
  bool groupFound = false;
  for (int i = 0; i < _metricGroupCount; i++) {
    if (_metricGroups[i].name == mName) {
      _metricGroups[i].help = mHelp;
      _metricGroups[i].type = mType;
      groupFound = true;
      break;
    }
  }
  if (!groupFound) {
    _metricGroups[_metricGroupCount].name = nm->name;
    _metricGroups[_metricGroupCount].help = mHelp;
    _metricGroups[_metricGroupCount].type = mType;
    _metricGroupCount++;
  }

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
  _metricGroupCount = 0;
};

String MetricsManager::getMetrics() {
  String result = "";

  // iterate per metric group
  for (int i = 0; i < _metricGroupCount; i++) {
    MetricGroup *actualGroup = &_metricGroups[i];
    result += "# HELP " + actualGroup->name + " " + actualGroup->help + "\n";
    result += "# TYPE " + actualGroup->name + " " + actualGroup->type + "\n";
    
    // iterate over metrics
    for (int i = 0; i < _metricCount; i++) {
      Metric *m = &_metrics[i];
      if (m->name != actualGroup->name) {
        continue;
      }
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
  }

  return result;
};
