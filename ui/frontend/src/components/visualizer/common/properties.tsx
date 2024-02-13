export const keyToLabelMap: Record<string, string> = {
  dbId: 'database ID',
  text: 'description',
}

export function propertyKeyToString(key: string) {
  return keyToLabelMap[key] || key
}

export function propertyValueToString(value: any) {
  if (value === true) {
    return 'true'
  }
  if (value === false) {
    return 'false'
  }
  return value
}

export function NodePropertyValue({ value, valueKey }: { value: string; valueKey: string }) {
  if (valueKey === 'url') {
    return (
      <a
        href={value}
        target="_blank"
        rel="noreferrer"
        style={{
          wordBreak: 'break-all',
        }}
      >
        {value}
      </a>
    )
  }

  if (valueKey === 'text') {
    return <span dangerouslySetInnerHTML={{ __html: value }} />
  }

  return propertyValueToString(value)
}
