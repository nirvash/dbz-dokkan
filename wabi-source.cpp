CardDatas CardModel::getMasterCardDatasByIds(const std::vector<uint32_t>& masterCardIds) const
{
  vector<CardDataPtr> results;
  results.resize(masterCardIds.size());
  size_t exists = 0;
  
  for (int i = 0; i < masterCardIds.size(); i++) {
    auto p = results[i] = _cardDataCache.fetch(masterCardIds[i]);
    if (p != nullptr) {
      exists++;
    }
  }
  
  if (masterCardIds.size() == exists) {
    return results;
  }
  
  string sql = form("SELECT * FROM cache.cards where id IN (%s);", join(masterCardIds, ", ").c_str());
  
  int i = 0;
  DatabaseManager::getInstance()->query(sql, [this, &results, &i](sqlite3_stmt* stmt) {
    if (results[i] == nullptr) {
      results[i] = _cardDataCache.store(make_shared<CardData>(stmt));
    }
    i++;
  });
  
  return results;
}