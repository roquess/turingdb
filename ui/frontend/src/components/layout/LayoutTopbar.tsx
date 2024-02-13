import IconArrowNarrowLeft from '@/assets/icons/arrow-narrow-left.svg'
import Link from 'next/link'

export default function LayoutTopbar() {
  return (
    <div className="border-b  border-l border-grey-900 bg-grey-800 p-4 text-sm">
      <Link
        className="inline-flex items-center gap-x-2 font-medium !text-content-secondary"
        href="/"
      >
        <IconArrowNarrowLeft /> Explore
      </Link>
    </div>
  )
}
