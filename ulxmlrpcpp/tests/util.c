
bool haveOption(int argc, char **argv, const char *name)
{
  for (int i = 0; i < argc; ++i)
  {
    if (strcmp(argv[i], name) == 0)
      return true;
  }
  return false;
}

