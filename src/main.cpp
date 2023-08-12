#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    ConverterJSON converterJson;
    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
    SearchServer searchServer(invertedIndex);
    std::vector<std::vector<std::pair<int, float>>> answers = searchServer.relativeIndexToAnswer(searchServer.search(converterJson.GetRequests(), converterJson.GetResponsesLimit()));
    converterJson.putAnswers(answers);
}
